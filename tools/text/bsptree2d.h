#ifndef BSPTREE_2D_H
#define BSPTREE_2D_H

#include <memory>
#include "image.h"

class BspTree2D {
    private:
        class Node;

    public:
        /* Default constructor */
        BspTree2D(Rect r);

        /* Destructor */
        virtual ~BspTree2D();

        /* No copy or move */
        BspTree2D(BspTree2D const& other) = delete;
        BspTree2D& operator=(BspTree2D const& other) = delete;
        BspTree2D(BspTree2D&& other) = delete;
        BspTree2D& operator=(BspTree2D&& other) = delete;

        std::unique_ptr<Node>& root();

    private:
        class Node {
            public:
                /* Default constructor */
                Node(Rect r);
                /* Destructor */
                virtual ~Node();

                /* No copy or move */
                Node(Node const& other) = delete;
                Node& operator=(Node const& other) = delete;
                Node(Node&& other) = delete;
                Node& operator=(Node&& other) = delete;

                void splitHorizontal(int y);
                void splitVertical(int x);

                Rect getRect() const;
                char getChar() const;
                void setChar(char c);

                /**
                 * \brief Try to fit a given rectangle in the Node.
                 *
                 * \param width Width of the rectangle to fit
                 * \param height Height of the rectangle to fit
                 *
                 * \return Reference to the Node where r was fit
                 * \throws If r is too big to fit in the Node
                 */
                Node& fit(int width, int height);

            private:
                Rect m_rect;
                char m_c;
                std::unique_ptr<Node> m_first;
                std::unique_ptr<Node> m_second;
        };

        std::unique_ptr<Node> m_root;
};

#endif // BSPTREE_2D_H
