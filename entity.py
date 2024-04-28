from qtpy.QtCore import QObject, QPointF, Signal, Property
from qtpy.QtGui import QColor


class Ripple(QObject):
    centerChanged = Signal(QPointF)
    radiusChanged = Signal(float)
    opacityChanged = Signal(int)
    expandFinished = Signal(bool)
    finished = Signal(bool)

    def __init__(self, center=QPointF(0, 0), radius=0, color=QColor(255, 255, 255, 100)):
        super().__init__()
        self._center = center
        self._radius = radius
        self._opacity = 255.0
        self._expand_finished = False
        self._finished = False

    def center(self):
        return self._center

    def setCenter(self, center):
        self._center = center
        self.centerChanged.emit(center)

    center = Property(QPointF, fget=center, fset=setCenter, notify=centerChanged)

    def radius(self):
        return self._radius

    def setRadius(self, radius):
        self._radius = radius
        self.radiusChanged.emit(radius)

    radius = Property(float, fget=radius, fset=setRadius, notify=radiusChanged)

    def opacity(self):
        return self._opacity

    def setOpacity(self, opacity):
        self._opacity = opacity
        self.opacityChanged.emit(opacity)

    opacity = Property(float, fget=opacity, fset=setOpacity, notify=opacityChanged)

    def isExpandFinished(self):
        return self._expand_finished

    def setExpandFinished(self, value):
        if not self._expand_finished and value:
            self.expandFinished.emit(value)
        self._expand_finished = value

    isExpandFinished = Property(bool, fget=isExpandFinished, fset=setExpandFinished, notify=expandFinished)

    def isFinished(self):
        return self._finished

    def setFinished(self, value):
        if not self._finished and value:
            self.finished.emit(value)
        self._finished = value

    isFinished = Property(bool, fget=isFinished, fset=setFinished, notify=finished)
