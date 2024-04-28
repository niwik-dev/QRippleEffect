import sys

from PySide6.QtGui import QColor
from PySide6.QtWidgets import QWidget, QApplication, QPushButton, QHBoxLayout, QVBoxLayout

from effect import RippleEffect
from option import RippleOption, StartPointOption


def RippleButton1(parent, color):
    btn = QPushButton(parent)
    btn.setStyleSheet("""
        background-color:transparent;
    """)
    btn.setFixedSize(200, 200)
    effect = RippleEffect(btn)
    option = RippleOption()
    option.rippleColor = color
    effect.setRippleOption(option)
    btn.setGraphicsEffect(effect)
    return btn


def RippleButton2(parent, color):
    btn = QPushButton(parent)
    btn.setStyleSheet("""
        background-color:transparent;
    """)
    btn.setFixedSize(200, 200)
    effect = RippleEffect(btn)
    option = RippleOption()
    option.rippleStartPoint = StartPointOption.Centered
    option.rippleColor = color
    effect.setRippleOption(option)
    btn.setGraphicsEffect(effect)
    return btn


class RippleTestWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.cols = QVBoxLayout(self)
        self.row1 = QHBoxLayout(self)
        self.cols.addLayout(self.row1)
        self.row1.addWidget(RippleButton1(self, QColor(255, 0, 0, 100)))
        self.row1.addWidget(RippleButton1(self, QColor(0, 255, 0, 100)))
        self.row1.addWidget(RippleButton1(self, QColor(0, 0, 255, 100)))

        self.row2 = QHBoxLayout(self)
        self.row2.addWidget(RippleButton2(self, QColor(255, 255, 0, 100)))
        self.row2.addWidget(RippleButton2(self, QColor(0, 255, 255, 100)))
        self.row2.addWidget(RippleButton2(self, QColor(255, 0, 255, 100)))

        self.cols.addLayout(self.row2)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = RippleTestWidget()
    widget.show()
    sys.exit(app.exec())
