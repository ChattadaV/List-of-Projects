import os

output=os.system('sudo apt-get install libhdf5-dev libhdf5-serial-dev libhdf5-100')
output=os.system('sudo apt-get install libqtgui4 libqtwebkit4 libqt4-test')
output=os.system('sudo apt-get install libatlas-base-dev')
output=os.system('sudo apt-get install libjasper-dev')


output=os.system('wget https://bootstrap.pypa.io/get-pip.py')
output=os.system('sudo python3 get-pip.py')

output=os.system('sudo pip3 install opencv-contrib-python==3.4.6.27')