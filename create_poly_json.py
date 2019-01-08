import cv2 as cv
import numpy as np
import json

def to_categorical(y, num_classes=None):
    y = np.array(y, dtype='int').ravel()
    if not num_classes:
        num_classes = np.max(y) + 1
    n = y.shape[0]
    categorical = np.zeros((n, num_classes))
    categorical[np.arange(n), y] = 1
    return categorical
    
img = cv.imread("images_test/Abbey_Road_watershed_mask.png")
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
config = json.load(open("config.json"))['labels']

shape = (gray.shape[0],gray.shape[1],1)
gray = gray.reshape(shape)
images = to_categorical(gray).reshape((gray.shape[0],gray.shape[1],-1))

out = open('images_test/Abbey_Road.json','w')
data = {}
data["version"] = "3.6.16"
data["flags"] = {}
data["shapes"] = []
data["lineColor"]= [0,255,0,128]
data["fillColor"]= [255,0,0,128]
data["imagePath"]= "Abbey_Road.jpg"
data["imageData"]= None
data["imageHeight"] = img.shape[0]
data["imageWidth"] = img.shape[1]
  
for label in config : 
    person = images[:,:,config[label]['id']]
    person[person>0] = 255

    person = person.astype('uint8')
    im, contour , hierarchy = cv.findContours(person, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    for i,c in enumerate(contour) :
        contour[i] = cv.approxPolyDP(c,1.5,True)
        d = {}
        d['label'] = label
        d["line_color"] = None
        d["fill_color"] = None
        if contour[i].shape[0] < 3:
            continue
        d["points"] = contour[i].reshape(contour[i].shape[0],contour[i].shape[2]).tolist()
        d["shape_type"] = "polygon"
        data["shapes"].append(d)

    # color = config[label]['color']
    # cv.drawContours(img, contour, -1, color, 1)
    
# cv.imshow("person",img)
# cv.waitKey(0)

json.dump(data,out,indent=2)