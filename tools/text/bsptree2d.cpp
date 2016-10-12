#include "bsptree2d.h"
#include <stdexcept>

using namespace std;

BspTree2D::Node::Node(Rect r) :
    m_rect(r),
    m_c(0),
    m_first(),
    m_second()
{ }

BspTree2D::Node::~Node() { }

void BspTree2D::Node::splitHorizontal(int y) {
    if(m_first || m_second)
        throw runtime_error("Only leaves can be split");
    else if(m_c != 0)
        throw runtime_error("Node contains a character");
    else if(y <= m_rect.y || y >= (m_rect.y + m_rect.height))
        throw runtime_error("Operation has no effect");
    Rect r1 = {m_rect.x, m_rect.y, m_rect.width, y - m_rect.y};
    Rect r2 = {m_rect.x, y, m_rect.width, m_rect.height - y + m_rect.y};
    m_first = make_unique<Node>(r1);
    m_second = make_unique<Node>(r2);
}

void BspTree2D::Node::splitVertical(int x) {
    if(m_first || m_second)
        throw runtime_error("Only leaves can be split");
    else if(m_c != 0)
        throw runtime_error("Node contains a character");
    else if(x <= m_rect.x || x >= (m_rect.x + m_rect.width))
        throw runtime_error("Operation has no effect");
    Rect r1 = {m_rect.x, m_rect.y, x - m_rect.x, m_rect.height};
    Rect r2 = {x, m_rect.y, m_rect.width - x + m_rect.x, m_rect.height};
    m_first = make_unique<Node>(r1);
    m_second = make_unique<Node>(r2);
}

Rect BspTree2D::Node::getRect() const { return m_rect; }

char BspTree2D::Node::getChar() const { return m_c; }

void BspTree2D::Node::setChar(char c) {
    if(m_first || m_second)
        throw runtime_error("Node is not a leaf");
    m_c = c;
}

BspTree2D::Node& BspTree2D::Node::fit(int width, int height) {
    if(m_first || m_second) {
        // If Node is not a leaf, fit recursively
        // TODO: maybe not rely on exceptions for "normal" execution
        try {
            return m_first->fit(width, height);
        }
        catch(runtime_error) {
            return m_second->fit(width, height);
        }
    }
    else {
        // Node is a leaf, compare against Node's rect
        if((width == m_rect.width) && (height == m_rect.height) && (m_c == 0))
            // Perfect fit, no split
            return *this;
        else if((width <= m_rect.width) && (height <= m_rect.height) && (m_c == 0)) {
            // Vertical or horizontal split?
            int dw = m_rect.width - width, dh = m_rect.height - height;
            if(dw > dh) {
                splitVertical(m_rect.x + width);
            }
            else {
                splitHorizontal(m_rect.y + height);
            }
            return m_first->fit(width, height);
        }
        else
            throw runtime_error("Cannot fit rectangle");
    }
}

BspTree2D::BspTree2D(Rect r) :
    m_root(make_unique<Node>(r))
{ }

BspTree2D::~BspTree2D() { }

unique_ptr<BspTree2D::Node>& BspTree2D::root() { return m_root; }
