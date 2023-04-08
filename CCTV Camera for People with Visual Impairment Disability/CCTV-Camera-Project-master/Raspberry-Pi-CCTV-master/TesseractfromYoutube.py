#----------From Youtube: https://youtube.com/watch?v=kxHp5ng6Rgw

import cv2
import PIL
from PIL import Image #from Python Imaging Libraries, import Image
import pytesseract #import image_to_text program
import numpy


img = Image.open("/home/pi/Downloads/harrypotter.png") #Directory of image file to extract text from

#Only include the following thresholding part to change image contrast
#thresh = 100
#fn = lambda x : 255 if x > thresh else 0
#img = img.convert('L').point(fn, mode='1') #
##img = img.convert('1')
#img.save('/home/pi/Downloads/camera_screenshot_02.11.2020_black.png')
#-----------------------------------------------


text = pytesseract.image_to_string(img, lang="eng") #use pytesseract to extract text in form of string, english langauge
print(text)

f = open('textauto.txt', 'w')
f.write(text)
f.close()
#----****--- Need to compile to make it automatic (if, elif, else) with buttons GUI

