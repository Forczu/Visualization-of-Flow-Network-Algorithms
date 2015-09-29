#include "EdgeImage.h"

EdgeImage::EdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: _vertexFrom(vertexFrom), _vertexTo(vertexTo), _context(&context)
{
}

EdgeImage::~EdgeImage()
{
}
