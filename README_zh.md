<h1 align="center">QRippleEffect🌊</h1>

<p align="center">简单，通用，可定制的 Qt/PyQt水波纹效果</p>

## 语言🌐

[简体中文](./README_zh.md)
[English](./README.md)

## 简介
<p>
  
  基于`QGraphicsEffect`实现的水波纹效果，可以应用于几乎所有控件上。只需要短短几行代码，即可使你的点击非常炫酷！

> 注：这个水波纹动画不是短触发动画，当长按不放开时会有保持反馈

```cpp
  label = QLabel() // 任何你想要的控件

  effect = RippleEffec(label)

  option = RippleOption() // 通过RippleOption对象实现自定义
  option.rippleColor = QColor(255,0,0,100)

  effect.setRippleOption(option)
  label.setGraphicsEffect(effect)
```

  目前支持的可定制项，见下表：

  <table>
    <tr>
      <th>属性</th>
      <th>说明</th>
      <th>描述</th>
    </tr>
    <tr>
      <td>rippleColor</td>
      <td>水波纹的颜色，默认白色</td>
      <td>任意颜色都可以</td>
    </tr>
    <tr>
      <td>acceptMouseButton</td>
      <td>接受的鼠标点击按钮，默认左键</td>
      <td>可以设置右键和中键</td>
    </tr>
    <tr>
      <td>startPointOption</td>
      <td>水波纹的开始位置，默认是鼠标点击处</td>
      <td>另外支持中央位置</td>
    </tr>
    <tr>
      <td>defaultAlphaValue</td>
      <td>默认的透明度，默认为100</td>
      <td>当水波纹颜色不透明时，透明度会默认设置这个值</td>
    </tr>
    <tr>
      <td>radiusAnimationDuration</td>
      <td>半径动画的时间</td>
      <td>数值为大，直观上感觉水波纹扩散越慢</td>
    </tr>
    <tr>
      <td>radiusAnimationEasingCurve</td>
      <td>半径动画的曲线，默认<code>QEasingCurve::OutQuad</code></td>
      <td>不同曲线有不同过渡效果</td>
    </tr>
    <tr>
      <td>opacityAnimationDuration</td>
      <td>透明动画的时间</td>
      <td>数值越大，直观上感觉水波纹消散越快</td>
    </tr>
    <tr>
      <td>opacityAnimationEasingCurve</td>
      <td>半径动画的曲线，默认<code>QEasingCurve::OutQuad</code></td>
      <td>不同曲线有不同过渡效果</td>
    </tr>
    <tr>
      <td>initialRadiusPercentage</td>
      <td>初始半径占父控件的百分比</td>
      <td>数值越大，感觉水波纹的初始尺寸越大；如果设置为0，则是从点开始扩散</td>
    </tr>
    <tr>
      <td>expandRadiusFinishPercentage</td>
      <td>水波纹拓展到总动画的百分之多少时，可以消散</td>
      <td>数值越小，直观感受越动画越短促；数值越大，直观感觉动画越连贯。取决于用户交互习惯</td>
    </tr>
  </table>

</p>

## 演示效果

![例子](https://github.com/Niwik-Dev/QRippleEffect/assets/168296107/daa73c9f-2d45-40df-a91d-2cf01b368721)

本样例见`test.cpp`文件
