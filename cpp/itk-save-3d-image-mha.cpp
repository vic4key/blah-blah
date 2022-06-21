#define VU_NO_EX
#include <vu>

typedef vu::point_3d_t<int> P3I;
typedef vu::point_3d_t<double> P3D;

#include <itkMetaImageIO.h>
#include <itkImageFileWriter.h>

namespace blabla
{

typedef itk::Image<float, 3> ImageType; // 3 dimensions + 2 bytes per pixel

#pragma comment(lib, "itksys-5.1.lib")
#pragma comment(lib, "ITKMesh-5.1.lib")
#pragma comment(lib, "ITKIOMeta-5.1.lib")
#pragma comment(lib, "ITKMetaIO-5.1.lib")
#pragma comment(lib, "ITKIOImageBase-5.1.lib")
#pragma comment(lib, "ITKIOTransformBase-5.1.lib")

ImageType::Pointer itk_create_image_3d(
  const ImageType::PixelType* ptr_pixel_data,
  const P3I& size,
  const P3D& origin,
  const P3D& spacing)
{
  auto result = ImageType::New();

  ImageType::PointType _origin;
  _origin[0] = origin.x(); // coordinates of the 
  _origin[1] = origin.y(); // first pixel in N-D
  _origin[2] = origin.z();
  result->SetOrigin(_origin);

  // Note: measurement units (e.g., mm, inches, etc.) are defined by the application.
  ImageType::SpacingType _spacing;
  _spacing[0] = spacing.x(); // spacing along X
  _spacing[1] = spacing.y(); // spacing along Y
  _spacing[2] = spacing.z(); // spacing along Z
  result->SetSpacing(_spacing);

  ImageType::IndexType _start;
  _start[0] = 0; // first index on X
  _start[1] = 0; // first index on Y
  _start[2] = 0; // first index on Z

  ImageType::SizeType _size;
  _size[0] = size.x(); // size along column
  _size[1] = size.y(); // size along row
  _size[2] = size.z(); // size along depth

  ImageType::RegionType region;
  region.SetIndex(_start);
  region.SetSize(_size);
  result->SetRegions(region);

  result->Allocate();

  int idx = 0;
  itk::ImageRegionIterator<ImageType> iter(result, region);
  while (!iter.IsAtEnd())
  {
    iter.Set(ptr_pixel_data[idx++]);
    ++iter;
  }

  return result;
}

void itk_save_image_3d(ImageType::Pointer image, std::string path)
{
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(path.c_str());
  writer->SetInput(image);
  writer->SetImageIO(itk::MetaImageIO::New());
  writer->Update();
}

void itk_save_image_3d(
  const ImageType::PixelType* ptr_pixel_data,
  const P3I& size,
  const P3D& origin,
  const P3D& spacing,
  std::string path)
{
  auto image = itk_create_image_3d(ptr_pixel_data, size, origin, spacing);
  itk_save_image_3d(image, path);
}

/*

itk_save_image(
  &FImageData[0],
  P3I(m_xFDim, m_yFDim, m_zFDim),
  P3D(m_xFImageStart, m_yFImageStart, m_zFImageStart),
  P3D(m_xFImageSpacing, m_yFImageSpacing, m_zFImageSpacing),
  "E:/image_fixed.mha"
);

*/

}; // namespace blabla