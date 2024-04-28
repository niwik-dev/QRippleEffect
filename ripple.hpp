#ifndef RIPPLE_EFFECT_HPP
#define RIPPLE_EFFECT_HPP

#include <QWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>

class Ripple: public QObject{
    Q_OBJECT
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(bool expand_finished READ isExpandFinished WRITE setExpandFinished NOTIFY isExpandFinished)
    Q_PROPERTY(bool finished READ isFinished WRITE setFinished NOTIFY isFinished)
protected:
    QPointF m_center;
    qreal m_radius;
    qreal m_opacity;
    bool m_expand_finished;
    bool m_finished;
public:
    Ripple(QPointF center=QPoint(0, 0), float radius=0, QColor color=Qt::GlobalColor::gray){
        m_center = center;
        m_radius = radius;
        m_opacity = 255.0;
        m_expand_finished = false;
        m_finished = false;
    }
public:
    QPointF center() const{
        return m_center;
    }
    QPointF setCenter(QPointF center){
        m_center = center;
        emit centerChanged(center);
        return center;
    }
    qreal radius() const{
        return m_radius;
    }
    qreal setRadius(float radius){
        m_radius = radius;
        emit radiusChanged(radius);
        return radius;
    }
    qreal opacity() const{
        return m_opacity;
    }
    qreal setOpacity(qreal opacity){
        m_opacity = opacity;
        emit opacityChanged(opacity);
        return opacity;
    }
    bool isExpandFinished() const{
        return m_expand_finished;
    }
    bool setExpandFinished(bool value){
        if(!m_expand_finished && value) {
            emit expandFinished(value);
        }
        m_expand_finished = value;
        return value;
    }
    bool isFinished() const{
        return m_finished;
    }
    bool setFinished(bool value){
        if(!m_finished && value) {
            emit finished(value);
        }
        m_finished = value;
        return value;
    }
signals:
    void centerChanged(QPointF center);
    void radiusChanged(float radius);
    void opacityChanged(int opacity);
    void expandFinished(bool expand_finished);
    void finished(bool finished);
};

enum class StartPointOption{
    Centered,
    Clicked
};

class RippleOption: public QObject{
public:
    QColor rippleColor = QColor(255,255,255,100);
    Qt::MouseButton acceptMouseButton = Qt::LeftButton;
    StartPointOption startPointOption = StartPointOption::Clicked;
    int defaultAlphaValue = 100;
    int radiusAnimationDuration = 1000;
    QEasingCurve radiusAnimationEasingCurve = QEasingCurve::OutQuad;
    int opacityAnimationDuration = 500;
    QEasingCurve opacityAnimationEasingCurve = QEasingCurve::OutQuad;
    float initialRadiusPercentage = 0.02;
    float expandRadiusFinishPercentage = 0.2;
};

class RippleEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(RippleOption* rippleOption READ rippleOption WRITE setRippleOption NOTIFY rippleOptionChanged)
protected:
    QList<Ripple*> m_ripples;
    RippleOption* m_ripple_option;
    QList<QPropertyAnimation*> m_opacity_animations;
    bool m_mouse_pressed;
public:
    explicit RippleEffect(QWidget* parent= nullptr,RippleOption* option= nullptr): QGraphicsEffect(parent) {
        m_mouse_pressed = false;
        if(parent){
            parent->installEventFilter(this);
        }
        if(option){
            m_ripple_option = option;
        }else{
            m_ripple_option = new RippleOption();
        }
    }

    void setParent(QWidget* parent){
        if(parent){
            parent->installEventFilter(this);
        }
    }

    void draw(QPainter *painter) override{
        QWidget*parent_widget = dynamic_cast<QWidget*>(parent());
        painter->setRenderHints(
            QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform |QPainter::TextAntialiasing
        );
        QPoint offset = QPoint();
        QPixmap pixmap = sourcePixmap(Qt::CoordinateSystem::LogicalCoordinates, &offset);
        if (pixmap.isNull()) {
            return;
        }
        painter->drawPixmap(offset, pixmap);

        painter->setPen(Qt::PenStyle::NoPen);
        QColor option_color = m_ripple_option->rippleColor;
        if (option_color.alpha() == 255) {
            option_color.setAlpha(m_ripple_option->defaultAlphaValue);
        }
        for (const Ripple *ripple: m_ripples) {
            QColor brush_color = QColor(option_color);
            float mix_alpha = brush_color.alphaF() * ripple->opacity() / 255.0;
            brush_color.setAlphaF(mix_alpha);
            if (!m_mouse_pressed) {
                for (QPropertyAnimation *opacity_animation: m_opacity_animations) {
                    if (opacity_animation->state() == QPropertyAnimation::State::Paused) {
                        opacity_animation->resume();
                    }
                }
            } else {
                for (QPropertyAnimation *opacity_animation: m_opacity_animations) {
                    if (opacity_animation->state() == QPropertyAnimation::State::Running) {
                        opacity_animation->pause();
                    }
                }
            }
            painter->setBrush(brush_color);
            painter->drawEllipse(ripple->center(), ripple->radius(), ripple->radius());
        }
        parent_widget->update();
    }

    RippleOption *rippleOption() const{
        return m_ripple_option;
    }

    void setRippleOption(RippleOption *ripple_option){
        m_ripple_option = ripple_option;
        emit rippleOptionChanged(ripple_option);
    }

    bool eventFilter(QObject *watched, QEvent *event) override{
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        QObject*parent = this->parent();
        QWidget* parent_widget = dynamic_cast<QWidget*>(parent);

        if(!parent_widget){
            return false;
        }
        if(!mouseEvent){
            return false;
        }
        if (mouseEvent&& !(m_ripple_option->acceptMouseButton & mouseEvent->button())) {
            return false;
        }

        switch (event->type()) {
            case QEvent::Type::MouseButtonRelease: {
                // 松开按钮时销毁完成动画的水波纹
                for (int index = 0; index < m_ripples.size(); ++index) {
                    Ripple *ripple = m_ripples.at(index);
                    if (ripple->isFinished()) {
                        m_ripples.removeAt(index);
                        m_opacity_animations.remove(index);
                        emit rippleRemoved(ripple);
                    }
                }
                m_mouse_pressed = false;
                break;
            }
            case QEvent::Type::MouseButtonPress:
            case QEvent::Type::MouseButtonDblClick: {
                // 点击（双击）按钮时创建水波纹以及动画
                Ripple *ripple;
                switch (m_ripple_option->startPointOption) {
                    case StartPointOption::Clicked: {
                        ripple = new Ripple(mouseEvent->position());
                        break;
                    }
                    case StartPointOption::Centered: {
                        if (parent_widget) {
                            ripple = new Ripple(parent_widget->rect().center());
                        }
                        break;
                    }
                }
                m_ripples.append(ripple);
                emit rippleAdded(ripple);

                QRectF rect = parent_widget->rect();
                float max_radius = std::sqrt(std::pow(rect.width(), 2) + std::pow(rect.height(), 2));
                QPropertyAnimation *radius_animation = new QPropertyAnimation(ripple, "radius");
                radius_animation->setDuration(m_ripple_option->radiusAnimationDuration);
                radius_animation->setEasingCurve(m_ripple_option->radiusAnimationEasingCurve);
                radius_animation->setStartValue(max_radius * m_ripple_option->initialRadiusPercentage);
                radius_animation->setEndValue(max_radius);

                if (m_ripple_option->expandRadiusFinishPercentage >= 1) {
                    connect(radius_animation, &QPropertyAnimation::finished, [=]() {
                        ripple->setExpandFinished(true);
                    });
                } else {
                    connect(radius_animation, &QPropertyAnimation::valueChanged, [=](const QVariant &value) {
                        float currentTask = value.toFloat() - radius_animation->startValue().toFloat();
                        float fullTask =
                                radius_animation->endValue().toFloat() - radius_animation->startValue().toFloat();
                        if (currentTask >= fullTask * m_ripple_option->expandRadiusFinishPercentage) {
                            ripple->setExpandFinished(true);
                        }
                    });
                }
                radius_animation->start();

                QPropertyAnimation *opacity_animation = new QPropertyAnimation(ripple, "opacity");
                opacity_animation->setDuration(m_ripple_option->opacityAnimationDuration);
                opacity_animation->setEasingCurve(m_ripple_option->opacityAnimationEasingCurve);
                opacity_animation->setStartValue(ripple->opacity());
                opacity_animation->setEndValue(0.0);
                m_opacity_animations.append(opacity_animation);
                connect(radius_animation, &QPropertyAnimation::finished, [=]() {
                    ripple->setFinished(true);
                });
                connect(ripple, &Ripple::expandFinished, [=]() {
                    opacity_animation->start();
                });
                m_mouse_pressed = true;
                break;
            }
            default:{
                break;
            }
        }
        return QObject::eventFilter(watched, event);
    }
signals:
    void rippleOptionChanged(RippleOption* ripple_option);
    void rippleAdded(Ripple* ripple);
    void rippleRemoved(Ripple* ripple);
};

#endif