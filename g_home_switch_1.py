#!/usr/bin/python
import json
import os
from sys import stdin, exit
from subprocess import call

#print os.getenv('CONTENT_TYPE'), os.getenv('CONTENT_LENGTH'), str(os.environ)
#content = json.dumps(content)
REQUEST_CONTENT_TYPE = 'application/json'
REQUEST_METHOD = 'post'

requestContentType = os.getenv('CONTENT_TYPE', '')
requestContentType = requestContentType.lower()
requestMethod = os.getenv('REQUEST_METHOD', '')
requestMethod = requestMethod.lower()

headers = '''Content-Type: application/json; charset=utf-8
Cache-Control: no-cache
pragma: no-cache
Content-Length: %d
'''

responseBodyObj = json.loads('''{
    "speech": "",
    "displayText": "",
    "data": {
        "google":{
            "expect_user_response":false
        }
    },
    "contextOut": [],
    "source": "CirelliIoT"
}''')

errorResponseObj = json.loads('''{
    "status": {
        "code": 200,
        "errorType": "",
        "errorDetails": ""
    }
}''')

contentParseError = False
try:
    requestObj = json.load(stdin)
except Exception as e:
    errorResponseObj['status']['exception'] = str(e)
    contentParseError = True

responseBody = ''

if(contentParseError == True):
    errorResponseObj['status']['code'] = 201
    errorResponseObj['status']['errorType'] = 'bad content'
    errorResponseObj['status']['errorDetails'] = 'There is no content in the request'
    responseBody =  json.JSONEncoder().encode(errorResponseObj)
elif(REQUEST_CONTENT_TYPE not in requestContentType):
    errorResponseObj['status']['code'] = 202
    errorResponseObj['status']['errorType'] = 'bad content type'
    errorResponseObj['status']['errorDetails'] = 'Content type must be \'' + REQUEST_CONTENT_TYPE + '\' received \'' + requestContentType + '\''
    responseBody =  json.JSONEncoder().encode(errorResponseObj)
elif(requestMethod != REQUEST_METHOD):
    errorResponseObj['status']['code'] = 203
    errorResponseObj['status']['errorType'] = 'bad request method'
    errorResponseObj['status']['errorDetails'] = 'This service only handles method ' + REQUEST_METHOD + ' method was sent as \'' + requestMethod + '\''
    responseBody =  json.JSONEncoder().encode(errorResponseObj)
else:
    PIN = '0'
    ON = '1'
    OFF = '0'
    action  = requestObj['result']['parameters']['action']
    device = requestObj['result']['parameters']['device']
    location = requestObj['result']['parameters']['location']
    speech = 'The %s in the %s is now %s.' % (device, location, action)
    
    if(action == 'on'):
        try:
            p = call('./on.sh')
        except Exception as e:
            responseBodyObj['status'] = {}
            responseBodyObj['status']['exception'] = str(e)

    elif(action == 'off'):
        try:
            p = call('./off.sh')
        except Exception as e:
            responseBodyObj['status'] = {}
            responseBodyObj['status']['exception'] = str(e)
    else:
        speech = 'I\'m sorry, I do not know how to perform that action.'
        responseBodyObj['status']['code'] = 204
        responseBodyObj['status']['errorType'] = 'unsupported action'
        responseBodyObj['status']['errorDetails'] = 'Unsupported action \'' + action + '\''

    responseBodyObj['speech'] = speech
    responseBodyObj['displayText'] = speech
    responseBody = json.JSONEncoder().encode(responseBodyObj)

print headers % len(responseBody)
print responseBody

if(os.getenv('G_HOME_DEBUG', 1) == 1):
    debugFile = open("/tmp/g_home/debug.txt", "w")
    debugFile.write('########################################\n')
    debugFile.write(str(os.environ) + '\n\n')
    debugFile.write(json.dumps(requestObj, indent=4) + '\n\n')
    debugFile.write(headers % len(responseBody))
    debugFile.write('\n' + responseBody + '\n')
    debugFile.write('########################################\n\n\n')
    debugFile.close()
