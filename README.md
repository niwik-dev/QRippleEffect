<h1 align="center">QRippleEffect🌊</h1>

<p align="center">Simple, Versatile, Customizable Qt/PyQt Water Ripple Effect</p>

## Language🌐

[简体中文](./README_zh.md)
[English](./README.md)

## Where's Python Version?

Here [QtForPython Branch](https://github.com/Niwik-Dev/QRippleEffect/tree/QtForPython)

## Brief Introduction
<p>
  
  The water ripple effect based on `QGraphicsEffect` can be applied to almost any control. It only takes a few lines of code to make your clicks really cool!

> Note: This water ripple animation is not a short trigger animation, there will be a holding feedback when the long press does not release.

```cpp
  label = QLabel(); // Any widget you want

  effect = RippleEffect(btn)

  option = RippleOption(); // The RippleOption object is used for customization
  option.rippleColor = QColor(255,0,0,100)

  effect.setRippleOption(option)
  label.setGraphicsEffect(effect)
```

  Currently supported customisable items, see the following table：

  <table>
    <tr>
      <th>Property</th>
      <th>Explaination</th>
      <th>Description</th>
    </tr>
    <tr>
      <td>rippleColor</td>
      <td>water ripple color, default white</td>
      <td>any color is okay</td>
    </tr>
    <tr>
      <td>acceptMouseButton</td>
      <td>Accepts mouse click button, default left button</td>
      <td>You can set the right and middle button</td>
    </tr>
    <tr>
      <td>startPointOption</td>
      <td>The start position of the water ripple, default is the mouse click</td>
      <td>Central location is also supported</td>
    </tr>
    <tr>
      <td>defaultAlphaValue</td>
      <td>Default transparency, default is 100</td>
      <td>When the color of the water ripple is opaque, the transparency defaults to this value</td>
    </tr>
    <tr>
      <td>radiusAnimationDuration</td>
      <td>Duration of Animation on Radius</td>
      <td>If the value is large, it intuitively feels that the water ripple diffuses more slowly</td>
    </tr>
    <tr>
      <td>radiusAnimationEasingCurve</td>
      <td>Curve of Animation on Radius, dafult<code>QEasingCurve::OutQuad</code></td>
      <td>Different curves have different transition effects</td>
    </tr>
    <tr>
      <td>opacityAnimationDuration</td>
      <td>Duration of Animation on Opacity</td>
      <td>The larger the value, the faster the water ripple visually dissipates</td>
    </tr>
    <tr>
      <td>opacityAnimationEasingCurve</td>
      <td>Curve of Animation on Opacity, dafult<code>QEasingCurve::OutQuad</code></td>
      <td>Different curves have different transition effects</td>
    </tr>
    <tr>
      <td>initialRadiusPercentage</td>
      <td>The initial radius as a percentage of the parent widget</td>
      <td>The larger the value, the larger the initial size of the perceived water ripple; If set to 0, it spreads from the point</td>
    </tr>
    <tr>
      <td>expandRadiusFinishPercentage</td>
      <td>Water ripples can dissipate when extended to the percentage of the total animation</td>
      <td>The smaller the value, the shorter the animation; The larger the value, the more coherent the animation will feel intuitively.It depends on user interaction habits</td>
    </tr>
  </table>

</p>

## Demonstration

![例子](https://github.com/Niwik-Dev/QRippleEffect/assets/168296107/daa73c9f-2d45-40df-a91d-2cf01b368721)

> The actual effect is not based on the Gif image. The frame drop problem of GifCam is quite serious. (laugh

See `test.py` for this sample
