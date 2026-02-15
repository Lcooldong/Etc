import cv2
import pytesseract
from pytesseract import Output


image = cv2.imread('start.png')
# image = cv2.imread('test_start.png')
rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

custom_oem_psm_config = '-l kor+eng --oem 3 --psm 11'
#text = pytesseract.image_to_string(gray_image, config=custom_oem_psm_config)
#print(text)
results = pytesseract.image_to_data(gray_image, config=custom_oem_psm_config, output_type=Output.DICT)
boxes = pytesseract.image_to_boxes(gray_image, config=custom_oem_psm_config, output_type=Output.DICT)

print(results)
print(boxes)

for i in range(0, len(results["text"])):
    x = results["left"][i]
    y = results["top"][i]

    w = results["width"][i]
    h = results["height"][i]
    text = results["text"][i]
    conf = int(results["conf"][i])

    if conf > 70:
        text = "".join(c if ord(c) < 128 else "" for c in text).strip()
        cv2.rectangle(image, (x, y), (x+w, y+h), (0, 255, 0), 2)
        cv2.putText(image, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 200), 2)



if text == '게임':
    print("GAME START")

while True:

    cv2.imshow('custom', image)
#    cv2.imshow('frame1', image)
#    cv2.imshow('frame1', rgb_image)
#    cv2.imshow('frame1', gray_image)

    key = cv2.waitKey(1) & 0xFF
    if key == 27:
        break
