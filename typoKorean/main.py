# pip install --upgrade azure-cognitiveservices-vision-computervision
# pip install requests
# pip install pillow

import os
import io
import json
import sys
import time
from array import array
from azure.cognitiveservices.vision.computervision import ComputerVisionClient
from azure.cognitiveservices.vision.computervision.models import OperationStatusCodes
from azure.cognitiveservices.vision.computervision.models import VisualFeatureTypes
from msrest.authentication import CognitiveServicesCredentials
import requests
from PIL import Image, ImageDraw, ImageFont

credential = json.load(open('credential.json'))
API_KEY = credential['API_KEY']
ENDPOINT = credential['ENDPOINT']

cv_client = ComputerVisionClient(ENDPOINT, CognitiveServicesCredentials(API_KEY))

# local image
images_folder = os.path.join(os.path.dirname(os.path.abspath(__file__)), "images")

read_image_path = os.path.join(images_folder, "test.jpg")
read_image = open(read_image_path, "rb")
read_response = cv_client.read_in_stream(read_image, raw=True)
read_operation_location = read_response.headers["Operation-Location"]
operation_id = read_operation_location.split("/")[-1]

while True:
    read_result = cv_client.get_read_result(operation_id)
    if read_result.status.lower() not in ['notstarted', 'running']:
        break
    print('Waiting for result...')
    time.sleep(10)

print("---------------------------------------------------------")

if read_result.status == OperationStatusCodes.succeeded:
    for text_result in read_result.analyze_result.read_results:
        for line in text_result.lines:
            print(line.text)
            print(line.bounding_box)
print()

print("##########################################################")

# remote image

image_url = 'https://raw.githubusercontent.com/MicrosoftDocs/azure-docs/master/articles/cognitive-services/Computer-vision/Images/readsample.jpg'
response = cv_client.read(url=image_url, Language='en', raw=True)
operationLocation = response.headers['Operation-Location']
operation_id = operationLocation.split('/')[-1]

while True:
    read_result = cv_client.get_read_result(operation_id)
    if read_result.status not in ['notStarted', 'running']:
        break
    time.sleep(1)

# Print the detected text, line by line
if read_result.status == OperationStatusCodes.succeeded:
    for text_result in read_result.analyze_result.read_results:
        for line in text_result.lines:
            print(line.text)
            print(line.bounding_box)
print()

print("End of Computer Vision quickstart.")

