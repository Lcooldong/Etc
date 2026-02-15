from subprocess import Popen
# pip install pyautogui
# pip install numpy
# pip install pypiwin32
# pip install opencv-python
# pip install pillow
# pip install pytesseract

import pyautogui
import win32gui

import numpy as np
import cv2
import time

import pytesseract
from pytesseract import Output

from PIL import Image, ImageGrab
from functools import partial


email_id = 'n_cooldong@naver.com'
password = 'ehdgml43'

genshin_program_path = r'C:\Program Files\Genshin Impact\launcher.exe'
ESC_KEY = 27
FRAME_RATE = 1
SLEEP_TIME = 1/FRAME_RATE
game_x = 0
game_y = 0
game_w = 0
game_h = 0
start_x = 0


class WindowCapture:
    def __init__(self, window_name, capture_rate):
        self.window_name = window_name
        self.wait_time = 1/capture_rate
        self.frame = self.screenshot()

    def screenshot(self):
        hwnd = win32gui.FindWindow(None, self.window_name)
        if not hwnd:
            raise Exception('Window not found: ' + self.window_name)

        left, top, right, bot = win32gui.GetClientRect(hwnd)
        x, y = win32gui.ClientToScreen(hwnd, (left, top))
        return cv2.cvtColor(
            np.asarray(
                pyautogui.screenshot(
                    region=(x, y,
                            *win32gui.ClientToScreen(hwnd, (right - x, bot - y))))), cv2.COLOR_RGB2BGR)


def capture_all():
    ImageGrab.grab = partial(ImageGrab.grab, all_screens=True)
    img = ImageGrab.grab()
    img.save(r'.\..\images\full_screen.png')


if __name__ == '__main__':
    Popen([genshin_program_path])
    screen_w, screen_h = pyautogui.size()
    print(screen_w, screen_h)
    time.sleep(3)
    #capture = WindowCapture("Genshin Impact", FRAME_RATE)
    #image_capture = capture.screenshot()

    while True:
        capture_all()

        image = cv2.imread(r'.\..\images\full_screen.png')
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, thresh1 = cv2.threshold(gray_image, 127, 200, cv2.THRESH_BINARY)
        blured_image = cv2.GaussianBlur(thresh1, (3, 3), 0)

        custom_oem_psm_config = '-l kor+eng --oem 3 --psm 11'
        results = pytesseract.image_to_data(blured_image, config=custom_oem_psm_config, output_type=Output.DICT)
        print(results)

        #boxes = pytesseract.image_to_boxes(thresh1, config=custom_oem_psm_config, output_type=Output.DICT)
        #print(boxes)

 #       while True:
#            cv2.imshow('frame1', thresh1)
#
 #           key = cv2.waitKey(1) & 0xFF
#            if key == ESC_KEY:
 #               break

        game_start_flag = 0
        for i in range(0, len(results["text"])):
            x = results["left"][i]
            y = results["top"][i]

            w = results["width"][i]
            h = results["height"][i]
            text = results["text"][i]
            conf = int(results["conf"][i])

            if conf > 80:
                if text == '재시작':
                    print('인터넷 연결 확인')
                    pyautogui.click(x + w/2, y + h/2)
                else:
                    if text == '게임':
                        print("================런쳐 실행====================")
                        print(x, y, w, h)
                        game_x = x
                        game_y = y
                        game_h = h
                        game_start_flag = game_start_flag + 1
                        print("====================================")
                    if text == '실행':  # 배열[i], [i+1]로 바꿔야함 - 이어진 순서
                        print("====================================")
                        print(x, y, w, h)
                        start_x = x
                        game_start_flag = game_start_flag + 1
                        print("====================================")

        if game_start_flag > 0:
            pyautogui.click((game_x + start_x)/2, game_y + game_h)
            break

    time.sleep(20)

    login_flag = 0
    while True:
        capture_all()
        image = cv2.imread(r'.\..\images\full_screen.png')
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, thresh2 = cv2.threshold(gray_image, 210, 255, cv2.THRESH_BINARY)

        results = pytesseract.image_to_data(thresh2, config=custom_oem_psm_config, output_type=Output.DICT)
        print(results)

        for i in range(0, len(results["text"])):
            x = results["left"][i]
            y = results["top"][i]

            w = results["width"][i]
            h = results["height"][i]
            text = results["text"][i]
            conf = int(results["conf"][i])

            if conf > 70:
                # if 'n_***ng@naver.com' in text:
                if 'PRESS' in text:
                    login_flag = 1
                    print("이미 로그인 되어있음")
                    break
                elif email_id not in text and login_flag == 0:  # 초기화 기준을 봐야함
                    login_flag = 4
                    print("=================이메일 변경===================")
                    pyautogui.click(x + 500, y + 20)
                    print("이메일 변경 좌표 : ", x, y, w, h)
                    for i in range(0, 20):
                        pyautogui.press('backspace', interval=0.01)
                    print("====================================")
                    time.sleep(1)
                    pyautogui.typewrite(email_id, interval=0.1)
                elif '이디' in text:
                    print("=================이메일 입력===================")
                    login_flag = 2
                    print("이메일 입력 좌표 : ", x, y, w, h)
                    pyautogui.click(x + w / 2, y + h / 2)
                    pyautogui.typewrite(email_id, interval=0.1)
                    print("====================================")
                elif '비밀번호' in text:
                    print("=================비밀번호 입력===================")
                    login_flag = 3
                    print("비밀번호 좌표 : ", x, y, w, h)
                    pyautogui.click(x + w / 2, y + h / 2)
                    #pyautogui.typewrite(['a', 'b', 'c'], interval=0.1)
                    pyautogui.typewrite(password, interval=0.1)
                    print("====================================")
                    break

        if login_flag == 1:
            print('로그인 되어 있음 -> 게임 시작')
            pyautogui.click(int(screen_w/2), int(screen_h/2))
            break
        elif login_flag >= 2 or login_flag < 4:
            print('아이디, 비밀 번호 입력 완료')
            after_id_pw_flag = 0
            end_flag = 0
            print('아이디, 비밀번호 입력 후 시작하기')
            while True:
                capture_all()
                image = cv2.imread(r'.\..\images\full_screen.png')
                gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
                ret3, thresh3 = cv2.threshold(gray_image, 210, 255, cv2.THRESH_BINARY)

                results = pytesseract.image_to_data(thresh3, config=custom_oem_psm_config, output_type=Output.DICT)
                print(results)

                for i in range(0, len(results["text"])):
                    x = results["left"][i]
                    y = results["top"][i]

                    w = results["width"][i]
                    h = results["height"][i]
                    text = results["text"][i]
                    conf = int(results["conf"][i])

                    if conf > 70:
                        if '회' in text:
                            print("=================회===================")
                            print(x, y, w, h)
                            game_x = x
                            game_y = y
                            after_id_pw_flag = 1
                            # cv2.rectangle(image, (game_x, game_y), (game_x + w, game_y + h), (0, 255, 0), 2)

                        elif 'Facebook' in text:
                            after_id_pw_flag = after_id_pw_flag + 1
                            print("==================Facebook==================")
                            print(x, y, w, h)
                            game_x = int((game_x + x) / 2)
                            print("Global", game_x, game_y)
                            print("====================================")
                        elif 'Twitter' in text:
                            print("==================Twitter==================")
                            print(x, y, w, h)
                            game_y = int((game_y + y) / 2)
                            game_w = w
                            game_h = h
                            after_id_pw_flag = after_id_pw_flag + 1
                            print("====================================")
                            end_flag = 1
                            break

                    if end_flag == 1:
                        print("Global", int(game_x), int(game_y), after_id_pw_flag)
                        break

                if after_id_pw_flag >= 3:
                    print('로그인 후 게임시작')
                    pyautogui.click(game_x + game_w / 2, game_y + game_h / 2)
                    break
            break


    time.sleep(2)
    while True:
        capture_all()
        time.sleep(2)

        image = cv2.imread(r'.\..\images\full_screen.png')
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, thresh4 = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)

        results = pytesseract.image_to_data(thresh4, config=custom_oem_psm_config, output_type=Output.DICT)
        print(results)

        loading_flag = 0
        for i in range(0, len(results["text"])):
            x = results["left"][i]
            y = results["top"][i]

            w = results["width"][i]
            h = results["height"][i]
            text = results["text"][i]
            conf = int(results["conf"][i])

            if conf > 70:
                if text == 'PRESS':
                    print("====================================")
                    print(x, y, w, h)
                    loading_flag = 1
                    print("====================================")

        time.sleep(1)

        if loading_flag == 1:
            print('접속 완료')
            pyautogui.click(screen_w / 2, screen_h / 2)
            time.sleep(10)
            break


