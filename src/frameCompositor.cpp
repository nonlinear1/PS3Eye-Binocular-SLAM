#include "frameCompositor.h"

void frameCompositor::setup()
{

}

/*

Use Mat mat = toCv (img); to treat ofImage as Mat, modify the mat, upload the modified pixels with img.update () to the GPU,
As another method, call toOf (mat, img) every time Mat is modified. In this case the texture will be respawned only when needed (eg when the size has changed).*/