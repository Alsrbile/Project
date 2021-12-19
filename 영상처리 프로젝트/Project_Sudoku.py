import cv2
import numpy as np

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(cv2.CAP_PROP_FRAME_WIDTH,  1000)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 960)
cap.set(cv2.CAP_PROP_FPS, 60)

if not cap.isOpened:
    print("Unable to read camera feed")
running =True

pts=[] # 점 저장
while running:
    ret, frame = cap.read(0)
    X, Y,_ = frame.shape
    frame1 = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    LEN = 60
    # 왼쪽위 검출
    frame2 = frame1[X//2-350:X//2-350+LEN,Y//2-350:Y//2-350+LEN]
    corners = cv2.goodFeaturesToTrack(frame2,1,0.001,5)
    if corners is not None:
        for i in corners:
            px,py = i.flatten() # i.ravel()
            px2, py2 = px+Y//2-350, py+X//2-350
            cv2.circle(frame,(int(px2),int(py2)),3,(0,0,255),-1)
            

    # 오른쪽 위 검출
    frame3 = frame1[X//2-350:X//2-350+LEN,Y//2+350-LEN:Y//2+350]
    corners3 = cv2.goodFeaturesToTrack(frame3,1,0.0001,5)
    if corners3 is not None:
        for i in corners3:
            px,py = i.flatten() # i.ravel()
            px3, py3 = px+Y//2+350-LEN, py+X//2-350
            cv2.circle(frame,(int(px3),int(py3)),3,(0,0,255),-1)
            

    # 왼쪽아래 검출
    frame4 = frame1[X//2+350-LEN:X//2+350,Y//2-350:Y//2-350+LEN]
    corners4 = cv2.goodFeaturesToTrack(frame4,1,0.01,5)
    if corners4 is not None:
        for i in corners4:
            px,py = i.flatten() # i.ravel()
            px4, py4 = px+Y//2-350, py+X//2+350-LEN
            cv2.circle(frame,(int(px4),int(py4)),3,(0,0,255),-1)

    # 오른쪽아래 검출
    frame5 = frame1[X//2+350-LEN:X//2+350,Y//2+350-LEN:Y//2+350]
    corners5 = cv2.goodFeaturesToTrack(frame5,1,0.01,5)
    if corners5 is not None:
        for i in corners5:
            px,py = i.flatten() # i.ravel()
            px5, py5 = px+Y//2+350-LEN, py+X//2+350-LEN
            cv2.circle(frame,(int(px5),int(py5)),3,(0,0,255),-1)

    cv2.rectangle(frame, (Y//2-350,X//2-350,700,700), (0,255,0),5)
    cv2.imshow('t',frame)

    k = cv2.waitKey(60) & 0xFF
    if k == 27:
        running =False 
    
# perspective 점들 저장
pts.append([px2,py2])
pts.append([px3,py3])
pts.append([px5,py5])
pts.append([px4,py4])
   
while True:
    cv2.imshow("My card", frame)
    key=cv2.waitKey(1)
    if key==27:
        break


X, Y, _= frame.shape # size of output
w=1000; h=1000
srcQuad = np.array(pts, np.float32)
dstQuad = np.array([[0, 0], [w-1, 0], [w-1, h-1], [0, h-1]], np.float32)

# perspective 변환
pers = cv2.getPerspectiveTransform(srcQuad, dstQuad)
frame = cv2.warpPerspective(frame, pers, (w, h))
RESULT = frame.copy() # 마지막에 출력할 사진
cv2.imshow("Result", frame)
cv2.waitKey(0)


''' 따온 스도쿠를 이진화 하기 '''
Sq = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
_, Sqt = cv2.threshold(Sq, 120, 255, cv2.THRESH_BINARY_INV)
Sql = cv2.Laplacian(Sqt,cv2.CV_8U,ksize=5)
Sq = cv2.dilate(Sql, (7,7), 1)
Sq = cv2.erode(Sq, (1,1), 1)
Sq = cv2.dilate(Sq, (1,1), 1)
Sq2 = cv2.dilate(Sq, (1,1), 1)
Sqt = cv2.erode(Sqt, (2,2), 1)
# 경험적으로 잘나온 방법으로 채워넣음


w,h,_= frame.shape
""" 선따기 """ 
kl = np.ones((6,6),np.uint8)                    # 커널
Sq = cv2.morphologyEx(Sq,cv2.MORPH_CLOSE,kl)    
#Sq = cv2.ximgproc.thinning(Sq) # 선 얇게하기

kernel_row = np.ones((1,int(w/3)), np.uint8)    # 1로 구성된 행 커널
kernel_col = np.ones((int(w/3),1), np.uint8)    # 1로 구성된 열 커널

Sq_r = cv2.erode(Sq, kernel_row, 2)             # 침식(행)
Sq_r = cv2.dilate(Sq_r, kernel_row, 2)          # 팽창(행)
Sq_c = cv2.erode(Sq, kernel_col, 2)             # 침식(열)
Sq_c = cv2.dilate(Sq_c, kernel_col, 2)          # 팽창(열)

# 선이 들어갈수있는 가로세로 직선 구하기




x_sq, y_sq = Sq2.shape
black = np.zeros((w,h), np.uint8)
""" 좌표 구해서 바둑판 배열만들고 숫자 따로 빼내기 """
centroids_r = np.linspace(0,w-1,10)
centroids_c = np.linspace(0,h-1,10)
LenQ = int((1000//9)/8)   # 선 사이의 길이 구하고 4로 나누기
print(w,h)
행좌표 = []

for i in range(0,10):       # 가로줄의 좌표 구하고 원본 이미지에서 줄 지우기
    cent_y = centroids_r[i]
    black[int(cent_y)] = 255
    행좌표.append(int(cent_y))
열좌표= []
for i in range(0,10):       # 가로줄의 좌표 구하기
    cent_x = centroids_c[i]
    black[:,int(cent_x)] = 255
    열좌표.append(int(cent_x))



  
def NumSelect(Input_img):
    """ Knn 숫자 찾기 """
    digits = cv2.imread('New_digits1.jpg', cv2.IMREAD_COLOR)
    digits = digits[20:]
    digits = cv2.cvtColor(digits, cv2.COLOR_BGR2GRAY)
    h, w = digits.shape[:2]
    h, w = digits.shape[:2]
    
    cells = [np.hsplit(row, w//20) for row in np.vsplit(digits, h//20)]
    cells = np.array(cells)
    train_images = cells.reshape(-1, 400).astype(np.float32)
    train_labels = np.repeat(np.arange(1,10), len(train_images)/9)
    
    
    knn = cv2.ml.KNearest_create()
    knn.train(train_images, cv2.ml.ROW_SAMPLE, train_labels)
    
    img_resize = cv2.resize(Input_img, (20, 20), interpolation=cv2.INTER_AREA)
    img_flatten = img_resize.reshape(-1, 400).astype(np.float32)
    _ret, res, _, _ = knn.findNearest(img_flatten, 3) # 3 : 예측에 사용할 인접한 데이터의 수

    
    # 6과 8 구분
    if  int(res[0, 0]) ==8 or int(res[0, 0]) ==3 or int(res[0, 0]) ==9:
        contours, hierarchy= cv2.findContours(Input_img, cv2.RETR_TREE,  cv2.CHAIN_APPROX_NONE)
        if len(hierarchy[0]) == 3:
            return 8
        elif len(hierarchy[0]) == 2: 
            return 9
        else:
            return 3

    return int(res[0, 0])

'''좌표 이용해서 숫자 사진들 빼오기'''
사진 = np.zeros((20,20), np.uint8)
숫자 = np.zeros((10-1,10-1), np.uint8)
# 하나 하나 숫자를 읽어서 저장하고 숫자가 아니면 0으로 저장하기

for i in range(len(열좌표)-1):
    for j in range(len(행좌표)-1):
        사진 = Sqt[열좌표[i]+LenQ:열좌표[i+1]-LenQ,행좌표[j]+LenQ:행좌표[j+1]-LenQ]
        
        Ar, Alabel, Astats, Acentroids = cv2.connectedComponentsWithStats(사진, connectivity=8)
        (xx, yy, ww, hh, dd) = (Astats[1] if len(Astats)>=2 else Astats[0])
        Test = 사진[yy:yy+hh, xx:xx+ww]
        U = int(LenQ/10*8)
        Test = np.pad(Test, (U,U), 'constant', constant_values=0) # zero padding
        cv2.imshow("number", Test)
        cv2.waitKey(10)
        if np.sum(Test) > 255*10:
            숫자[i][j] = NumSelect(Test)

print(숫자)

# 스도쿠 구간
zeropos=[(i,j) for i in range(9) for j in range(9) if 숫자[i][j]==0]


def get_ans(x,y):
    numlist=[i for i in range(1,10)]
    for i in range(9):
        if 숫자[x][i] in numlist:
            numlist.remove(숫자[x][i])
        if 숫자[i][y] in numlist:
            numlist.remove(숫자[i][y])
    x=x//3
    y=y//3
    for i in range(x*3,(x+1)*3):
        for j in range(y*3,(y+1)*3):
            if 숫자[i][j] in numlist:
                numlist.remove(숫자[i][j])
    return numlist
# 숫자 행렬 복사
MATRIX = 숫자.copy()

def dfs(count):
    if count==len(zeropos):
        # 정답 복사해오기
        global MATRIX
        MATRIX = 숫자.copy()
        # 숫자에 해당하는 값을 중심에 집어넣기
        print(MATRIX)
        return 
    (x,y)=zeropos[count]
    candi=get_ans(x,y)
    for num in candi:
        숫자[x][y]=num
        dfs(count+1)
        숫자[x][y]=0

dfs(0) # 백트래킹으로 답구하기

    

# 스도쿠 숫자 표시하기
img1 = cv2.imread('NUMBER/1.jpg', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('NUMBER/2.jpg', cv2.IMREAD_GRAYSCALE)
img3 = cv2.imread('NUMBER/3.jpg', cv2.IMREAD_GRAYSCALE)
img4 = cv2.imread('NUMBER/4.jpg', cv2.IMREAD_GRAYSCALE)
img5 = cv2.imread('NUMBER/5.jpg', cv2.IMREAD_GRAYSCALE)
img6 = cv2.imread('NUMBER/6.jpg', cv2.IMREAD_GRAYSCALE)
img7 = cv2.imread('NUMBER/7.jpg', cv2.IMREAD_GRAYSCALE)
img8 = cv2.imread('NUMBER/8.jpg', cv2.IMREAD_GRAYSCALE)
img9 = cv2.imread('NUMBER/9.jpg', cv2.IMREAD_GRAYSCALE)

# 이진화
_, img1 = cv2.threshold(img1, 120, 255, cv2.THRESH_BINARY_INV)
img1 = cv2.cvtColor(img1, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img1.shape
(b,g,r)=cv2.split(img1)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img1=cv2.merge((b,g,r))

_, img2 = cv2.threshold(img2, 120, 255, cv2.THRESH_BINARY_INV)
img2 = cv2.cvtColor(img2, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img2.shape
(b,g,r)=cv2.split(img2)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img2=cv2.merge((b,g,r))

_, img3 = cv2.threshold(img3, 120, 255, cv2.THRESH_BINARY_INV)
img3 = cv2.cvtColor(img3, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img3.shape
(b,g,r)=cv2.split(img3)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img3=cv2.merge((b,g,r))

_, img4 = cv2.threshold(img4, 120, 255, cv2.THRESH_BINARY_INV)
img4 = cv2.cvtColor(img4, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img4.shape
(b,g,r)=cv2.split(img4)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img4=cv2.merge((b,g,r))

_, img5 = cv2.threshold(img5, 120, 255, cv2.THRESH_BINARY_INV)
img5 = cv2.cvtColor(img5, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img5.shape
(b,g,r)=cv2.split(img5)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img5=cv2.merge((b,g,r))

_, img6 = cv2.threshold(img6, 120, 255, cv2.THRESH_BINARY_INV)
img6 = cv2.cvtColor(img6, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img6.shape
(b,g,r)=cv2.split(img6)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img6=cv2.merge((b,g,r))

_, img7 = cv2.threshold(img7, 120, 255, cv2.THRESH_BINARY_INV)
img7 = cv2.cvtColor(img7, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img7.shape
(b,g,r)=cv2.split(img7)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img7=cv2.merge((b,g,r))

_, img8 = cv2.threshold(img8, 120, 255, cv2.THRESH_BINARY_INV)
img8 = cv2.cvtColor(img8, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img8.shape
(b,g,r)=cv2.split(img8)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img8=cv2.merge((b,g,r))

_, img9 = cv2.threshold(img9, 120, 255, cv2.THRESH_BINARY_INV)
img9 = cv2.cvtColor(img9, cv2.COLOR_GRAY2BGR)
rows,cols,chs=img9.shape
(b,g,r)=cv2.split(img9)
g=np.zeros((rows,cols,1),dtype="uint8")
b=np.zeros((rows,cols,1),dtype="uint8")
img9=cv2.merge((b,g,r))

# 사이즈변경하기
img1 = cv2.resize(img1, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img2 = cv2.resize(img2, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img3 = cv2.resize(img3, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img4 = cv2.resize(img4, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img5 = cv2.resize(img5, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img6 = cv2.resize(img6, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img7 = cv2.resize(img7, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img8 = cv2.resize(img8, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)
img9 = cv2.resize(img9, (LenQ*6, LenQ*6), interpolation=cv2.INTER_AREA)

def IMGselect(xy):
    if xy == 1:
        return img1
    elif xy == 2:
        return img2    
    elif xy == 3:
        return img3
    elif xy == 4:
        return img4  
    elif xy == 5:
        return img5  
    elif xy == 6:
        return img6  
    elif xy == 7:
        return img7
    elif xy == 8:
        return img8
    elif xy == 9:
        return img9

(Lx, Ly) = Sqt.shape
print(Lx, Ly)
Linx = Lx//(11-2)
Liny = Lx//(11-2)

# 결과 출력창
Sq1 = cv2.cvtColor(Sqt, cv2.COLOR_GRAY2BGR)
for i in range(0,9):
    for j in range(0,9):
        if 숫자[i][j] == 0:
            xy = MATRIX[i][j]
            imgS = IMGselect(xy)
            # 이진화하고 더하기
            imgS1 = cv2.cvtColor(imgS, cv2.COLOR_BGR2GRAY)
            _,imgS2 = cv2.threshold(imgS1, 20, 255, cv2.THRESH_BINARY_INV)
            ROI_1 = frame[Linx*i+LenQ:Linx*i+int(LenQ*7),Liny*j+LenQ:Liny*j+int(LenQ*7)]
            RES1 = cv2.bitwise_and(ROI_1, ROI_1, mask=imgS2)
            frame[Linx*i+LenQ:Linx*i+int(LenQ*7),Liny*j+LenQ:Liny*j+int(LenQ*7)] = cv2.add(imgS,RES1)
_, Sq1 = cv2.threshold(Sq1, 125, 255, cv2.THRESH_BINARY_INV)
Sq1 = cv2.resize(Sq1, (800,800), interpolation=cv2.INTER_AREA)
frame = cv2.resize(frame, (800,800), interpolation=cv2.INTER_AREA)
RESULT = cv2.resize(RESULT, (800,800), interpolation=cv2.INTER_AREA)


cv2.imshow('ff', frame)
cv2.imshow('Result', RESULT)

cv2.waitKey(0)
cap.release()
cv2.destroyAllWindows()
