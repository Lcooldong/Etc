from subprocess import Popen
import pyautogui
import win32gui

import numpy as np
import cv2
import time

import pytesseract
from pytesseract import Output

from PIL import Image, ImageGrab
from functools import partial

genshin_program_path = r'C:\Program Files\Genshin Impact\launcher.exe'
ESC_KEY = 27
FRAME_RATE = 1
SLEEP_TIME = 1/FRAME_RATE
game_x = 0
game_y = 0
game_h = 0
start_x = 0
target_threshold = 180

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
    img.save(r'.\..\..\images\full_screen.png')


if __name__ == '__main__':
    screen_w, screen_h = pyautogui.size()
    middle_w = int(screen_w / 2)
    middle_h = int(screen_h / 2)
    print(middle_w, middle_h)

    image = cv2.imread(r'.\..\..\images\template_matching.png', cv2.IMREAD_COLOR)
    dst = image.copy()
    roi = image[middle_w -100:600, middle_w + 100: 700]
    dst[0:800, 0:800] = roi
    # dst = image[middle_w: middle_h, middle_w + 200: middle_h + 200].copy()

    # gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # hsv = cv2.cvtColor(dst, cv2.COLOR_BGR2HSV)
    # h, s, v = cv2.split(hsv)
    # contours, hierarchy = cv2.findContours(h, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    #
    # ret, threshold = cv2.threshold(h, 100, 255, cv2.THRESH_BINARY)
    # binary = cv2.bitwise_not(threshold)
    # blured_image = cv2.GaussianBlur(threshold, (3, 3), 0)
    #
    # find_threshold_flag = 0

    # for i in range(len(contours)):
    #     cv2.drawContours(image, [contours[i]], 0, (0, 0, 255), 2)
    #     cv2.putText(image, str(i), tuple(contours[i][0][0]), cv2.FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 0), 1)
    #     print(i, hierarchy[0][i])
    #     cv2.imshow("origin", image)
    #     cv2.waitKey(0)

    while True:
        cv2.imshow('frame1', dst)
        # cv2.imshow("h", h)
        # cv2.imshow("s", s)
        # cv2.imshow("v", v)

        key = cv2.waitKey(1) & 0xFF
        if key == ESC_KEY:
            break

