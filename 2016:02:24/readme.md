<big>

</br>

->國立臺灣科技大學 資訊工程研究所<-

</br></br></br>

-><big>圖學導論上課筆記 week 1</big><-

</br></br></br>

->修課學生：王澤浩<-

->授課教師：賴祐吉 博士<-

</br></br></br>

->日期：2016 年 02 月 29 日<-

</big>

<div style="page-break-after: always;"></div>

<div style="text-align: justify;">

## 什麼是 Computer Graphics?

名詞的定義是「凡舉經電腦生成並顯示出的影像」皆可被稱作為 Computer Graphics。

## OpenGL

OpenGL 提供開發者進行簡單至複雜的二維、三維空間景象繪製，經常被應用於遊戲開發、資料視覺化、CAD 等。而近年火熱的遊戲引擎 Unity3D 便是調教 OpenGL 及包裝成 scripts 以利其他開發者進行的開發工作。

## Entertainment

Computer Graphics 最常見於娛樂產業中，不論是電影場景生成、動畫製作、遊戲開發等。而前述兩者多為了要減少製作成本，以及增加未來的重複可用性，甚至能品質能夠超越實景進行拍攝。

->![witcher3-gameplay](img/witcher3-gameplay.png =450x)<-

->巫師 3 遊戲畫面<-

## User Interface

從螢幕上的使用 Graphical User Interface 延伸涉足到行動裝置、穿戴式設備的 Human Computer Interaction，皆可見 Computer Graphics 都佔有一席之地。

->![iron-man-2-jarvis](img/iron-man-2-jarvis.jpg =450x)<-

->Iron Man 2 劇照，J.A.R.V.I.S. 的操作介面運用了圖學技術<-


## Virtual Reality 與 Augmented Reality

提到 VR 就會想到 [Oculus VR](https://www.oculus.com/) 所開發的 Oculus Rift 頭戴式顯示器，近年來在各大遊戲展中展露頭角，期待能為未來的遊戲市場注入一股活力。然而 VR 不僅限於遊戲產業，許多醫學團隊也著手將該技術導入醫療服務市場，如醫療人員的培訓等。

->![vr-in-medi](img/vr-in-medi.jpg =450x)<-

->醫學團隊使用 VR 技術進行人員培訓<-

而 Augmented Reality 與 Computer Vision 的關係較為密切，這部分個人認為在未來幾年會突發性地成長，越來越講求數據化的情況下，AI Learning 相關的技術蓬勃發展，便可更加精準的辨識圖像中的人、事、物。

->![ar-is-potential](img/ar-is-potential.jpg =450x)<-

->期待未來 AR 技術能夠即時性的辨識各種事物<-

## Raster image 與 Vector image

Raster image 以點陣式的方式記錄圖像所有的像素點，若將圖片不斷地放大變會發現圖像產生鋸齒失真的情形。而 Vector image 利用向量的方式記錄圖像，舉例來說，一個三角形圖案若在 raster image 則必須要記錄當前解析度的全部座標點的 RGB，vector image 僅需記錄三點的座標比例，依照當前解析度再行運算出結果後上色。也因為大量運算的關係，在早期硬體設備還不夠支援的情形下並不廣為使用。


## CUDA

利用 NVIDIA CUDA 技術將繪圖的計算以平行化處理進行加速，而近年來 GPU Programming 的技術也不僅侷限於 Computer Graphics 領域，以現今的數據導向的資訊社會中，CUDA 的技術亦被應用於 Machine Learning、Deep Learning 之中。

->![deep-neural-networks](img/deep-neural-networks.png =650x)<-

->人臉影像辨識應用於類神經網路<-

## 結語

課堂上老師提到了一個很有趣的現象，人眼接受到的並非我們常聽到的 RGB 三原色，Blue 的部分被某一種近似於藍色的能量給取代，課後上網查但找不到相關的資料，實屬可惜。

我雖然使用 macbook retina，但我的眼睛實在分辨不出 retina 跟一般螢幕的解析程度差異，如果說 retina 設計上是增加螢幕像素密度到平均人類的極限值，照這樣子看來我應該是平均之下呀。

本次上課為第一週上課，實驗室的大夥們給了我滿滿醍醐灌頂。希望藉由這門課能夠學習到更豐富的圖學知識，並且磨練程式技巧。

---

<div style="text-align: left;">

本課程 homeworks、projects 整理於：[https://github.com/grass0916/NTUST-1042-CG](https://github.com/grass0916/NTUST-1042-CG)