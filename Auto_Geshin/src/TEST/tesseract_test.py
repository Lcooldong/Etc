import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'
# oem : Engine mode, psm : 글자 인식 설정
config = '-l kor+eng --oem 3 --psm 11'
text = pytesseract.image_to_string(Image.open('start.png'), config=config)
print('==========텍스트 인식 결과==========')
print(text)

