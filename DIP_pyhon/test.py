import numpy as np
import cv2

img = cv2.imread("./yeast-cells.tif")
cv2.imshow("Test",img)
cv2.waitKey(0)
cv2.destroyAllWindows()
