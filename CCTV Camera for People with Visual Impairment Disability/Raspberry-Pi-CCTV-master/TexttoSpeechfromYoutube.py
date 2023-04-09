#--------From Youtube: https://www.youtube.com/watch?v=_Q8wtPCyMdo&list=PLh2AuTfro4t4tQSAsjrTJ5XBsVffh1TzS&index=6

from gtts import gTTS #import text_tp_speech program (Google Text to Speech)
import os

#myText = 'Testing Text to Speech on Raspberry Pi' #for text_to_speech in the quote
fh = open('textauto.txt', 'r') #file handling (open text document for text_tp_speech)
myText = fh.read().replace('\n', ' ') #set myText to the the saved document and replace all the line endings with a space (to not confuse gTTS)


language = 'en' #set language

output = gTTS(text=myText, lang=language, slow=False) #setting output text_to_speech file

output.save('/home/pi/Downloads/output.mp3') #saving output file
fh.close() #close file handling
output = os.system('mpg321 /home/pi/Downloads/output.mp3') #start output file
#^^^^^^^^^^^^Need to fix 'start' in the line above


#---****---Need to compile to make it automatic (if, elif, else) with buttons 
#---****---Need to make Google Text to Speech (gTTS) offline
