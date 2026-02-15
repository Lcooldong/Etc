# pillow, opencv-python, numpy, huggingface_hub, onnxruntime, onnx
# 쿠다 코어 사용 환경 X => 미구현 => cuda toolkit 11.7 + cudnn  8.5.0.96

import os
import re
import sys

import cv2
import natsort
import numpy as np
import huggingface_hub
import onnxruntime as rt
from PIL import Image
import moviepy.editor as mp
import zipfile
import torch
import time

from optimum.onnxruntime import ORTModelForSequenceClassification
from transformers import AutoTokenizer

file_output_name = "frame"
output_video_name = "result.mp4"
output_audio_name = "sound.mp3"
output_mix = ''
codec_type = 'mp4v'

root_dir = './images'
mask_dir = './mask'
res_dir = './result'
video_dir = './video'
music_dir = './music'
onlyVideo_dir = './result/onlyVideo'
zip_dir = ''        # 받을 압축 파일 위치 => 공유를 위한 포멧
file2zip_dir = ''   # 압축할 경로


possible_img_extension = ['.jpg', '.jpeg', '.JPG', '.bmp', '.png']
possible_video_extension = ['.avi', '.mp4', '.mkv', '.wmv', '.flv']


# 모든 파일 압축
def compress2_zip(output_dir):
    owd = os.getcwd()     # working directory
    os.chdir(output_dir)  # change directory 해당 폴더로 이동

    zip_file = zipfile.ZipFile(file2zip_dir + '\\', 'w') # 쓰기
    for (path, dirs, files) in os.walk(output_dir):
        for file in files:
            if file.endswith('.png'):
                zip_file.write(os.path.join(path, file), compress_type=zipfile.ZIP_DEFLATED)

    zip_file.close()
    os.chdir(owd)   # 원래 working directory 로 되돌림


# 압축 해제 => 공유 파일 사용
def decompress_zip(input_dir, output_dir):
    output_unzip = zipfile.ZipFile(input_dir, 'r')
    output_unzip.extractall(output_dir)
    for (path, dirs, files) in os.walk(output_dir):
        for _dir in dirs:
            if _dir == video_dir:    #
                pass
    output_unzip.close()


def get_video_name_list(_dir):
    video_path_list = []
    video_name_list = []
    for (root, dirs, files) in os.walk(_dir):
        if len(files) > 0:
            for file_name in files:
                if os.path.splitext(file_name)[1] in possible_video_extension:
                    img_path = root + '/' + file_name
                    img_path = img_path.replace('\\', '/')
                    video_path_list.append(img_path)
                    video_name_list.append(file_name[:-4])
    # print(video_name_list)

    return video_name_list, video_path_list


# 다 작업하기는 하지만 35 350 3500 순으로 리스트 순서가 일정하지 않음 => 00001 처럼 앞에 0으로 채우면됨
def get_image_list(_dir):
    img_path_list = []
    file_name_list = []
    _cnt = 0
    for (root, dirs, files) in os.walk(_dir):
        if len(files) > 0:
            for file_name in files:
                if os.path.splitext(file_name)[1] in possible_img_extension:
                    img_path = root + '/' + file_name
                    img_path = img_path.replace('\\', '/')
                    img_path_list.append(img_path)
                    file_name_list.append(file_name)
                    # print(file_name)

    # a = os.listdir(_dir)
    # print(natsort.natsorted(a))
    file_name_list = natsort.natsorted(file_name_list)  # 파일명 정렬
    img_path_list = natsort.natsorted(img_path_list)

    # print(file_name_list)
    # print(img_path_list)
    return file_name_list, img_path_list


# 투명한 배경 검은색_으로 만들어 주는 함수
def change_color(_dir):

    _files, _paths = get_image_list(_dir)
    print('img count :' + str(len(_files)))

    for _file in _files:
        print(str(_file))
        im = Image.open('./images/' + str(_file))
        # im.save(str(_file) + '.png')
        # img = Image.open(str(_file) + '.png')
        img = im.convert("RGBA")
        new_data = []
        datas = img.getdata()  # 각 픽셀 RGBA 값

        transparent_cutoff = 5  # 투명 인식 기준값
        color_cutoff = 50       # 검정 인식 기준값

        for item in datas:
            # 투명 -> 검정
            if item[3] < transparent_cutoff:
                new_data.append((0, 0, 0, 255))
            # 검정 -> 투명
            # if item[0] < color_cutoff and item[1] < color_cutoff and item[2] < color_cutoff:
            #     new_data.append((0, 0, 0, 0))
            else:
                new_data.append(item)

        img.putdata(new_data)
        img.save('./result/'+str(_file), "PNG")
        # img.show('./result/'+str(_file))


# 영상을 1 프레임 씩 나눔
def divide_video(_dir):
    video_name = ''
    video_path = ''
    for (root, dirs, files) in os.walk(_dir):
        if len(files) > 0:
            for file_name in files:
                if os.path.splitext(file_name)[1] in possible_video_extension:
                    _path = root + '/' + file_name
                    _path = _path.replace('\\', '/')
                    video_path = _path
                    video_name = video_path[:-4]    # 확장자만 제거한 경로
                    print('Path : ' + video_path)
                    print('Name : ' + video_name)
        else:
            print('please input your video.')

    _video = cv2.VideoCapture(video_path)

    if not _video.isOpened():
        print("Could not Open Video")
        exit()

    length = int(_video.get(cv2.CAP_PROP_FRAME_COUNT))
    width = int(_video.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(_video.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = _video.get(cv2.CAP_PROP_FPS)

    print("length :", length)
    print("width :", width)
    print("height :", height)
    print("fps :", fps)

    try:
        if not os.path.exists(video_name):
            os.makedirs(video_name)
            print("Create New Directory")
    except OSError:
        print('Error: Creating directory. ' + video_name)

    # 영상 실행 테스트
    # while _video.isOpened():
    #     ret, frame = _video.read()
    #     if ret:
    #         # re_frame = cv2.resize(frame, (round(width/4), round(height/4)))
    #         re_frame = cv2.resize(frame, (width, height))
    #         cv2.imshow('VIDEO', re_frame)
    #         key = cv2.waitKey(10)
    #
    #         if key == ord('q'):
    #             break
    #     else:
    #         print("Got Error")
    #         break
    #
    # _video.release()
    # cv2.destroyAllWindows()
    count = 0
    while _video.isOpened():
        ret, image = _video.read()

        # [1초당]
        # if int(_video.get(1)) % fps == 0:  # 앞서 불러온 fps 값을 사용하여 1초마다 추출
        #     cv2.imwrite(video_name + "/frame%d.png" % count, image)
        #     print('Saved frame number :', str(int(_video.get(1))))
        #     count += 1

        # [1프레임당] - 1.0 => 프레임 단위임 60.0 으로 하면 1초
        if int(_video.get(1)) % 1.0 == 0:  # 앞서 불러온 fps 값을 사용하여 1초마다 추출
            cv2.imwrite(video_name + "/" + file_output_name + "%05d.png" % count, image)    # 파일명 중요
            print('Saved frame number :', str(int(_video.get(1))) + ' / ' + str(length))
            count += 1
            if count == length:
                break
        else:
            print('Cannot make frames')
    _video.release()
    print('Dividing Video Complete')
    return video_name


# Declare Execution Providers
providers = ['CUDAExecutionProvider', 'CPUExecutionProvider']

# Download and host the model
model_path = huggingface_hub.hf_hub_download("skytnt/anime-seg", "isnetis.onnx")
rmbg_model = rt.InferenceSession(model_path, providers=providers)
# rmbg_model = rt.InferenceSession(("skytnt/anime-seg"), providers=['CUDAExecutionProvider'])


# Function to get mask
def get_mask(img, s=1024):
    # Resize the img to a square shape with dimension s
    # Convert img pixel values from integers 0-255 to float 0-1
    img = (img / 255).astype(np.float32)
    # get the amount of dimensions of img
    dim = img.shape[2]
    # Convert the input image to RGB format if it has an alpha channel
    if dim == 4:
        img = img[..., :3]
        dim = 3
    # Get height and width of the image
    h, w = h0, w0 = img.shape[:-1]
    # IF height is greater than width, set h as s and w as s*width/height
    # ELSE, set w as s and h as s*height/width
    h, w = (s, int(s * w / h)) if h > w else (int(s * h / w), s)
    # Calculate padding for height and width
    ph, pw = s - h, s - w
    # Create a 1024x1024x3 array of 0's
    img_input = np.zeros([s, s, dim], dtype=np.float32)
    # Resize the original image to (w,h) and then pad with the calculated ph,pw
    img_input[ph // 2:ph // 2 + h, pw //
              2:pw // 2 + w] = cv2.resize(img, (w, h))
    # Change the axes
    img_input = np.transpose(img_input, (2, 0, 1))
    # Add an extra axis (1,0)
    img_input = img_input[np.newaxis, :]
    # Run the model to get the mask
    mask = rmbg_model.run(None, {'img': img_input})[0][0]
    # Transpose axis
    mask = np.transpose(mask, (1, 2, 0))
    # Crop it to the images original dimensions (h0,w0)
    mask = mask[ph // 2:ph // 2 + h, pw // 2:pw // 2 + w]
    # Resize the mask to original image size (h0,w0)
    mask = cv2.resize(mask, (w0, h0))[:, :, np.newaxis]
    return mask


# Function to remove background
def rmbg_fn(img):
    # Call get_mask() to get the mask
    mask = get_mask(img)
    # Multiply the image and the mask together to get the output image
    img = (mask * img + 255 * (1 - mask)).astype(np.uint8)
    # Convert mask value back to int 0-255
    mask = (mask * 255).astype(np.uint8)
    # Concatenate the output image and mask
    img = np.concatenate([img, mask], axis=2, dtype=np.uint8)
    # Stacking 3 identical copies of the mask for displaying
    mask = mask.repeat(3, axis=2)
    return mask, img


def gather_mask(_dir, _target_dir):
    image_count = 0
    _video_name_list, _video_path_list = get_video_name_list(_dir)
    for _video_file in _video_name_list:
        _files, _paths = get_image_list(_dir)
        print(_video_file + " Image Length : " + str(len(_files)))
        for _img_file in _files:
            # print(_img_file)
            # print(_dir + '/' + _video_file + '/' + 'frame325.png')
            print(_dir + '/' + _video_file + '/' + _img_file)
            # print(_img_file)
            _img = cv2.imread(_dir + '/' + _video_file + '/' + _img_file)
            # _img = cv2.imread(_dir + '/' + _video_file + '/' + 'frame512.png')    # test

            n_mask, n_img = rmbg_fn(np.array(_img))   # nmask 마스크 nimg 배경 제거한 그림

            cv2.imwrite(mask_dir + '/' + _img_file, n_mask)
            #
            # cv2.imshow('result', _img)
            # cv2.imshow('MASK', n_mask)
            # cv2.waitKey(0)
            # break

            # image_count = image_count + 1
            # if image_count == 100:
            #     break
    print("Creating Mask Complete")


def make_video(_dir, _fps):
    file_length = 0
    size = 0
    frame_array = []
    _files, _paths = get_image_list(_dir)
    file_list = os.listdir(_dir)
    file_length = len(file_list)
    print("Total files : %d" % file_length)

    # _paths = [os.path.join(_dir, i) for i in os.listdir(_dir) if re.search(".png$", i)]
    print('Read files : ', end='')

    last_percent = 0
    for idx, path in enumerate(_paths):
        img = cv2.imread(path)
        height, width, layers = img.shape
        size = (width, height)
        frame_array.append(img)

        _percent = "{}%".format(round(float(idx / file_length) * 100), 4)
        # print(f'{float(idx / file_length) * 100:.1f}')

        float_percent = float(idx / file_length) * 100
        if int(float_percent) > last_percent:
            last_percent = int(float_percent)
            print('■', end='')

    print('|')

    # if (codec_type == 'mp4v'):
    _video_name_list, _video_path_list = get_video_name_list(video_dir)

    output_file = _video_name_list[0]
    
    # make onlyVideo path 
    try:
        if not os.path.exists(onlyVideo_dir):
            os.makedirs(onlyVideo_dir)
            print("Create New Directory")
    except OSError:
        print('Error: Creating directory. ' + onlyVideo_dir)

    out = cv2.VideoWriter(onlyVideo_dir + '/' + output_file + '.mp4',
                          cv2.VideoWriter_fourcc(*codec_type), float(_fps), size)
    result_path = onlyVideo_dir + '/' + output_file + '.mp4'

    last_percent = 0
    print('Make video : ', end='')
    for i in range(len(frame_array)):
        out.write(frame_array[i])
        _percent = "{}%".format(round(float(i / file_length)*100), 3)
        float_percent = float(i / file_length) * 100
        if int(float_percent) > last_percent:
            last_percent = int(float_percent)
            print('■', end='')
        # print("Making Progress : " + str(_percent))
    out.release()
    print('|')
    print("Making Video Done")
    return result_path


def get_video_data(_dir):
    print('Get Video Data from' + _dir)


# 비디오 + 음악 생성
def mix_video_music(_images_dir, _fps):
    _onlyVideo_path = make_video(_images_dir, _fps)    # 추후에 여러 파일 가능하게 해야함
    _music_paths = extract_music(video_dir, music_dir)  # 리스트
    print('video : ' + _onlyVideo_path + '| music :' + str(len(_music_paths)))
    video_clip = mp.VideoFileClip(_onlyVideo_path)

    _video_name_list, _video_path_list = get_video_name_list(video_dir)
    file_list = os.listdir(_images_dir)
    file_length = len(file_list)
    duration = round(file_length / _fps)
    print('Duration : ' + str(duration))    # 지속 시간

    audio_clip = mp.AudioFileClip(_music_paths[0]).subclip(0, duration)  # subclip(0, -2) cut last two second, 시간을 나타냄
    video_clip.audio = audio_clip
    print('Save mix_file at ' + res_dir + '/' + _video_name_list[0] + '.mp4')
    video_clip.write_videofile(res_dir + '/' + _video_name_list[0] + '.mp4')

    print('Mix Done')

    # for idx, _music_path in enumerate(_music_paths):
    #     print('music file path : ' + _music_path)
    #     audio_clip = mp.AudioFileClip(_music_path).subclip(1, file_length)
    #     video_clip.audio = audio_clip
    #     video_clip.write_videofile(res_dir + _video_path_list[idx])


def reset_folder(_dir):
    for (root, dirs, files) in os.walk(_dir):
        if len(files) > 0:
            for file_name in files:
                if os.path.splitext(file_name)[1] in possible_img_extension:
                    img_path = root + '/' + file_name
                    img_path = img_path.replace('\\', '/')
                    os.remove(img_path)

    return print('Remove Done')


# 여러 파일 동시에 추출 가능한 코드 다른 곳에 적용 해야함
def extract_music(_dir, _target_dir):
    _video_name_list, _video_path_list = get_video_name_list(_dir)
    _music_file_path = []

    for idx, _video_path in enumerate(_video_path_list):
        _video = cv2.VideoCapture(_video_path)

        length = int(_video.get(cv2.CAP_PROP_FRAME_COUNT))
        width = int(_video.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(_video.get(cv2.CAP_PROP_FRAME_HEIGHT))
        fps = _video.get(cv2.CAP_PROP_FPS)
        _min = (length / fps) / 60
        _sec = (length / fps) % 60
        _time = "{0:02d}m {1:0.1f}s".format(int(_min), _sec)
        print('Total Time : ' + _time)

        if not _video.isOpened():
            print("Could not Open Video")
            exit()
        else:
            clip = mp.VideoFileClip(_video_path)
            print('\nExtract to =>' + _target_dir + '/' + _video_name_list[idx] + '.mp3')
            clip.audio.write_audiofile(_target_dir + '/' + _video_name_list[idx] + '.mp3')
            _music_file_path.append(_target_dir + '/' + _video_name_list[idx] + '.mp3')
            # print('Extract Music Done')
            _video.release()
    print(_music_file_path)
    return _music_file_path


def progressbar():
    sys.stdout.write('\nProgress : [')
    sys.stdout.flush()
    for i in range(0, 100):
        sys.stdout.write('■' * int(i / 50))
        sys.stdout.flush()
    sys.stdout.write(']\n')
    sys.stdout.flush()


if __name__ == '__main__':
    # img_path_list = []
    # file_name_list = []
    # _cnt = 0
    # for (root, dirs, files) in os.walk(root_dir):
    #     if len(files) > 0:
    #         for file_name in files:
    #             if os.path.splitext(file_name)[1] in possible_img_extension:
    #                 img_path = root + '/' + file_name
    #                 img_path = img_path.replace('\\', '/')
    #                 img_path_list.append(img_path)
    #                 file_name_list.append(file_name)
    #
    # print(file_name_list)

                    # print(file_name)
    # get_video_name_list(video_dir)
    # change_color(root_dir)
    # reset_images_folder()

    # 사용 함수
    # 영상 나누기 + 마스킹 => 추출 함수로 통합 해야함
    divide_video(video_dir)           # 영상 나누기
    gather_mask(video_dir, mask_dir)  # 마스크 만들기

    # make_video(root_dir, 60.0)        # 동영상 만들기 음원 X
    # extract_music(video_dir, music_dir)
    # mix_video_music(root_dir, 60.0)     # 영상 총집합
    # check_gpu_provider()
    #
    # print(torch.__version__)
    # print(torch.cuda.is_available())
    # print(torch.version.cuda)
    # print(torch.cuda.device_count())  # GPU 사용 가능 개수
    # print(torch.cuda.get_device_name(0))
