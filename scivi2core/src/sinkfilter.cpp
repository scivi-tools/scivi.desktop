#include "sinkfilter.h"

namespace scivi {
namespace filters {

SinkFilter::SinkFilter(QObject *parent) : FilterImplementation(parent) {}

SinkFilter::~SinkFilter() { m_container = nullptr; }

void SinkFilter::setContainer(QQuickItem *view) { m_container = view; }

}  // namespace filters
}  // namespace scivi
