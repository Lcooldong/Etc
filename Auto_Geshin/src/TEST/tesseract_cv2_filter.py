import cv2
import pytesseract
from pytesseract import Output


image = cv2.imread('start.png')
# image = cv2.imread('test_start.png')
rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

blurred = cv2.GaussianBlur(gray_image, (3, 3), 0)
edged = cv2.Canny(blurred, 75, 200)

custom_oem_psm_config = '-l kor+eng --oem 3 --psm 12'
#text = pytesseract.image_to_string(gray_image, config=custom_oem_psm_config)
#print(text)
results = pytesseract.image_to_data(gray_image, config=custom_oem_psm_config, output_type=Output.DICT)
boxes = pytesseract.image_to_boxes(edged, config=custom_oem_psm_config, output_type=Output.DICT)

print(results)
print(boxes)


for i in range(0, len(results["text"])):
    x = results["left"][i]
    y = results["top"][i]

    w = results["width"][i]
    h = results["height"][i]
    text = results["text"][i]
    conf = int(results["conf"][i])

    if conf > 90:
        if text == '게임':
            print("====================================")
            cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
            print(x, y, w, h)
            print("====================================")
        if text == '실행':
            print("====================================")
            cv2.rectangle(image, (x, y), (x+w, y+h), (0, 255, 0), 2)
            print(x, y, w, h)
            print("====================================")

        text = "".join(c if ord(c) < 128 else "" for c in text).strip()
        cv2.putText(image, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 200), 2)




#for i in range(0, len(results["text"])):
#    text = results["text"][i]
#    print(text)
#    if text == '게임':
#        print("====================================")
#    if text == '실행':
#        print("====================================")



while True:

#    cv2.imshow('blured', blurred)
#    cv2.imshow('edged', edged)
    cv2.imshow('frame1', image)
#    cv2.imshow('frame1', rgb_image)
#    cv2.imshow('frame1', gray_image)

    key = cv2.waitKey(1) & 0xFF
    if key == 27:
        break
