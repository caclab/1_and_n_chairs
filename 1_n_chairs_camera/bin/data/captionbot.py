import requests   
import random  
from requests.packages.urllib3.exceptions import InsecureRequestWarning  
from requests.packages.urllib3.exceptions import InsecurePlatformWarning
import sys
import json

requests.packages.urllib3.disable_warnings(InsecureRequestWarning)  
requests.packages.urllib3.disable_warnings(InsecurePlatformWarning)

frame = sys.argv[1]

def getCaption(image, key): 
	_url = 'https://api.projectoxford.ai/vision/v1.0/describe'

	headers = { 'Content-Type': 'application/octet-stream',
	            'Ocp-Apim-Subscription-Key': key }

	with open(image, 'rb') as stream:
	    response = requests.post(_url, headers=headers,
	        data=stream.read())
	    response = response.json()

	description = response['description']['captions'][0]['text']
	confidence = response['description']['captions'][0]['confidence']

	return (description, confidence)

data = getCaption(frame, '8430c23616204473b317dfb938dbf75e')

print str(data[0])