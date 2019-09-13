#ifndef VISUAL_H
#define VISUAL_H

#include <QtQuick/QQuickItem>

namespace scivi {
namespace filters {

class Visual {
public:
    virtual ~Visual() {}
    virtual void render(QQuickItem *container) = 0;
};

}
}

#endif // VISUAL_H
