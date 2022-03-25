#include "bitmap.hpp"

#include <algorithm>
#include <array>
#include <deque>

using namespace std;

struct Bitmap::BitmapImpl
{
    char* pixels_;
    size_t size_;

    BitmapImpl(char* image, size_t size)
        : pixels_ {image}
        , size_ {size}
    {
    }

    void render()
    {
       for (size_t i = 0; i < size_; ++i)
        cout << pixels_[i]; 
    }

    BitmapImpl(const BitmapImpl&) = delete;

    ~BitmapImpl()
    {
        delete[] pixels_;
    }
};

Bitmap::~Bitmap()
{
}

Bitmap::Bitmap(size_t size, char fill_char)
    : pimpl_ {std::make_unique<Bitmap::BitmapImpl>(new char[size], size)}
{
    fill_n(pimpl_->pixels_, pimpl_->size_, fill_char);
}

void Bitmap::draw()
{
    cout << "Image: ";
    pimpl_->render();
    cout << endl;
}
