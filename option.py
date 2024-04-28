from dataclasses import dataclass
from enum import Enum, unique

from PySide6.QtCore import Qt, QEasingCurve
from PySide6.QtGui import QColor


@unique
class StartPointOption(Enum):
    Centered = 0
    Clicked = 1


@dataclass
class RippleOption:
    rippleColor = QColor(255, 255, 255, 100)
    acceptMouseButton = Qt.MouseButton.LeftButton
    rippleStartPoint = StartPointOption.Clicked
    defaultAlphaValue = 100
    radiusAnimationDuration = 1000
    radiusAnimationEasingCurve = QEasingCurve.Type.OutQuad
    opacityAnimationDuration = 500
    opacityAnimationEasingCurve = QEasingCurve.Type.OutQuad
    initialRadiusPercentage = 0.02
    expandRadiusFinishPercentage = 0.2
