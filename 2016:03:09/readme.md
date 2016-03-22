<big>

</br>

->國立臺灣科技大學 資訊工程研究所<-

</br></br></br>

-><big>圖學導論上課筆記 week 3</big><-

</br></br></br>

->修課學生：王澤浩<-

->授課教師：賴祐吉 博士<-

</br></br></br>

->日期：2016 年 03 月 10 日<-

</big>

<div style="page-break-after: always;"></div>

<div style="text-align: justify;">

## Dithering

因為早期圖像資料格式的緣故，無法儲存圖片中的全部色彩，因此在有限的儲存空間中，必須犧牲掉出現頻率較少的色彩。由於能夠顯示的色彩較少，直接影響到了肉眼在觀看圖片的感受，dithering 便是為了減緩其中的不自然性所衍生的應用技術。其中 Random modulation 更是為了貼近大眾人類的審美觀，刻意破壞 image 的結構換取觀看期的舒適感。

以下為常見的幾種 Dithering 方式：

* Naïve Threshold Algorithm
	* intensity 門檻值為 0.5，將顏色劃分為黑白二色
* Brightness Preserving Algorithm
	* intensity 門檻值為全部 pixel 之平均
	* 比 naive 更加明顯
* Random Modulation
	* 在計算 intensity 時，加上隨機亂數，刻意營造出邊界的模糊化
* Floyd-Steinberg Dithering
	* 較前述 dithering 中呈現效果最佳者
	* 利用與原值差異的 error 比例影響後繼點

->![Dithering algorithms](img/Dithering_algorithms.png =550x)<-

->Dithering 演算法<-

## Point processing

圖中的 x 直代表 RGB 三原色同時做某一運算，可以看到比 original 小者將會變暗，反之變亮。而 invert 負片則是三原色上限 255 的補數。

->![Point processing](img/Point_processing.png =550x)<-

->Point 的個別運算<-

## Linear interpolation in image warping

今天若要對 image 做縮放、翻轉或是歪斜等特效，除了最簡易的鄰近點顏色外，可使用線性內插取得近似點之顏色，其為 Bilinear interpolation 雙線性內插。

* 由四點構成 q11, q12, q21, q22 四點構成之矩形範圍，求範圍內特定點之值。
* 1.0 / (x2x1 * y2y1) * (q11 * x2x * y2y + q21 * xx1 * y2y + q12 * x2x * yy1 + q22 * xx1 * yy1)

->![Bilinear interpolation & NN](img/Linear-interpolation.png =350x)<-

->雙線性內插法與最鄰近取點<-

---

<div style="text-align: left;">

本課程 homeworks、projects 整理於：[https://github.com/grass0916/NTUST-1042-CG](https://github.com/grass0916/NTUST-1042-CG)