<big>

</br>

->國立臺灣科技大學 資訊工程研究所<-

</br></br></br>

-><big>圖學導論上課筆記 week 2</big><-

</br></br></br>

->修課學生：王澤浩<-

->授課教師：賴祐吉 博士<-

</br></br></br>

->日期：2016 年 03 月 07 日<-

</big>

<div style="page-break-after: always;"></div>

<div style="text-align: justify;">

## RGB and CMYK

提到了光的三原色 RGB 不免就會聯想到印刷四原色 CMYK，光的 RGB 疊加後會得到白色，但印刷的 CMY 疊加後得到得卻是黑色的 K。因為我們說的 **RGB 是釋出能量方**「Light energy」，**CMYK 則屬於吸收能量方**。

->![RGB v.s. CMYK](img/RGBvsCMYK.jpg =220x)<-

->RGB v.s. CMYK<-

## Hue

色相 hue 用來區分色彩間的顏色，像是「祖母綠」、「橄欖綠」、「天空藍」、「玫瑰金」和「太空灰」等 ... 使用了其它現實物體的借喻方式來描述一色彩，而上述提到的名詞皆可歸於色相的一環。

->![dressing](img/dressing.jpg =150x)<-

->色彩恆定性：不同光源下，究竟是「黑+藍」還是「白+金」的洋裝？<-

## Image compression

以 GIF 為例，在 **GIF 的格式中會有 Color Table** 記錄這張圖片的色彩。然而這個 Color Table 是有空間限制的，於是採用了有損壓縮，將依區域當中的近似色彩使用幾樣 color 取代繁多的 color。像是 **JPEG 使用了 Fourier Transform** 將影像進行壓縮。其它的有損影像格式大致上原理相同，因肉眼辨識不易，所以適用於自然攝影的圖像中。

->![lossless v.s. lossy](img/lossless-vs-lossy.png =450x)<-

->少量壓縮與大量壓縮影像之比較<-

## HSL and HSV

在常見的小畫家、Photo Shop 之中都有調色盤、選色器的工具，而相關的工具 UI 五花八門，但其實都是對 Hue + Saturation + (Value / Lightness) = RGB 去做變化。而我個人叫好奇的是，透明度 alpha 算不算表示色彩的一種方式之一？

->![HSL v.s. HSV](img/HSL-HSV-cylinder.png =400x)<-

->HSL 與 HSV 的色域<-

## Z buffering（bonus）

這週提到了影像儲存的原理，在 Computer Graphics 中，我們所見的圖像無法直接從物體的顏色辨識出深淺，所以我們需要再生成圖片的同時，將 Z 軸（也就是深度）暫存起來，於是得其名 Z buffer。而在其它的領域中，像是 Kinect 在捕捉使用者的 RGB 顏色外，利用深度感應器得知使用者肢體的相對距離等，也是將原只有 x-y 兩軸的空間，再更進一步模擬出第三軸 Z 軸的存在。

->![Z buffer](img/z-buffer.png =500x)<-

->Z buffer 深度緩衝<-

---

<div style="text-align: left;">

本課程 homeworks、projects 整理於：[https://github.com/grass0916/NTUST-1042-CG](https://github.com/grass0916/NTUST-1042-CG)