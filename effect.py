import math

from qtpy.QtCore import QPoint, Qt, Signal, QObject, QEvent, QRect, QPropertyAnimation
from qtpy.QtGui import QPainter, QColor, QMouseEvent
from qtpy.QtWidgets import QGraphicsEffect, QWidget

from entity import Ripple
from option import RippleOption, StartPointOption


class RippleEffect(QGraphicsEffect):
    rippleOptionChanged = Signal(RippleOption)
    rippleAdded = Signal(Ripple)
    rippleRemoved = Signal(Ripple)

    def __init__(self, parent=None, option=None):
        super().__init__(parent)
        self.m_ripples = []
        self.m_ripple_option = option or RippleOption()
        self.m_opacity_animations: list[QPropertyAnimation] = []
        self.m_mouse_pressed = False
        if parent:
            parent.installEventFilter(self)

    def setParent(self, parent):
        if parent:
            parent.installEventFilter(self)

    def draw(self, painter: QPainter) -> None:
        parent_widget = self.parent()
        painter.setRenderHints(
            QPainter.RenderHint.Antialiasing |
            QPainter.RenderHint.SmoothPixmapTransform |
            QPainter.RenderHint.TextAntialiasing, True
        )
        offset = QPoint()
        pixmap = self.sourcePixmap(
            Qt.CoordinateSystem.LogicalCoordinates, offset
        )
        painter.drawPixmap(offset, pixmap)

        painter.setPen(Qt.PenStyle.NoPen)
        option_color = self.m_ripple_option.rippleColor
        if option_color.alpha() == 255:
            option_color.setAlpha(self.m_ripple_option.defaultAlphaValue)

        for ripple in self.m_ripples:
            brush_color = QColor(option_color)
            mix_alpha = brush_color.alphaF() * ripple.opacity / 255.0
            brush_color.setAlphaF(mix_alpha)

            if not self.m_mouse_pressed:
                for opacity_animation in self.m_opacity_animations:
                    if opacity_animation.state() == QPropertyAnimation.State.Paused:
                        opacity_animation.resume()
            else:
                for opacity_animation in self.m_opacity_animations:
                    if opacity_animation.state() == QPropertyAnimation.State.Running:
                        opacity_animation.pause()

            painter.setBrush(brush_color)
            painter.drawEllipse(
                ripple.center, ripple.radius, ripple.radius
            )
        parent_widget.update()

    def setRippleOption(self, option: RippleOption):
        self.m_ripple_option = option

    def eventFilter(self, watched: QObject, event: QEvent) -> bool:
        if isinstance(event, QMouseEvent):
            mouse_event = event
            if not mouse_event.button() & self.m_ripple_option.acceptMouseButton:
                return False
        else:
            return False

        parent_widget = self.parent()

        if event.type() == QEvent.Type.MouseButtonRelease:
            for index, ripple in enumerate(self.m_ripples):
                if ripple.isFinished:
                    self.m_ripples.pop(index)
                    self.m_opacity_animations.pop(index)
                    self.rippleRemoved.emit(ripple)

            self.m_mouse_pressed = False
        elif event.type() in (
                QEvent.Type.MouseButtonPress,
                QEvent.Type.MouseButtonDblClick,
        ):
            if self.m_ripple_option.rippleStartPoint == StartPointOption.Clicked:
                center = event.position()
            elif self.m_ripple_option.rippleStartPoint == StartPointOption.Centered:
                if parent_widget and isinstance(parent_widget,QWidget):
                    center = parent_widget.rect().center()
                else:
                    center = event.position()
            else:
                raise ValueError("Invalid StartPointOption")

            ripple = Ripple(center=center)
            self.m_ripples.append(ripple)
            self.rippleAdded.emit(ripple)

            rect: QRect = parent_widget.rect()
            max_radius = math.sqrt(math.pow(rect.width(), 2) + math.pow(rect.height(), 2))
            radius_animation = QPropertyAnimation(ripple, b"radius")
            radius_animation.setStartValue(self.m_ripple_option.initialRadiusPercentage * max_radius)
            radius_animation.setEndValue(max_radius)
            radius_animation.setDuration(self.m_ripple_option.radiusAnimationDuration)
            radius_animation.setEasingCurve(self.m_ripple_option.radiusAnimationEasingCurve)

            if self.m_ripple_option.expandRadiusFinishPercentage >= 1.0:
                radius_animation.finished.connect(lambda: ripple.setExpandFinished(True))
            else:
                def onValueChanged(value):
                    current_task = float(value) - float(radius_animation.startValue())
                    full_task = float(radius_animation.endValue()) - float(radius_animation.startValue())
                    if current_task / full_task >= self.m_ripple_option.expandRadiusFinishPercentage:
                        ripple.setExpandFinished(True)

                radius_animation.valueChanged.connect(onValueChanged)

            radius_animation.start()

            opacity_animation = QPropertyAnimation(ripple, b"opacity")
            self.m_opacity_animations.append(opacity_animation)
            opacity_animation.setStartValue(ripple.opacity)
            opacity_animation.setEndValue(0)
            opacity_animation.setDuration(self.m_ripple_option.opacityAnimationDuration)
            opacity_animation.setEasingCurve(self.m_ripple_option.opacityAnimationEasingCurve)
            opacity_animation.finished.connect(lambda: ripple.setFinished(True))
            ripple.expandFinished.connect(lambda: opacity_animation.start())
            self.m_mouse_pressed = True

        return super().eventFilter(watched, event)
