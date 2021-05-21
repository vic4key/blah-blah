#define VU_NO_EX
#include <vu>

typedef vu::Point3DT<int> P3I;
typedef vu::Point3DT<double> P3D;

#include <itkMetaImageIO.h>
#include <itkImageFileWriter.h>

typedef itk::Image<short, 3> ITKImageType;

#pragma comment(lib, "itksys-4.4.lib")
#pragma comment(lib, "ITKMesh-4.4.lib")
#pragma comment(lib, "ITKIOMeta-4.4.lib")
#pragma comment(lib, "ITKMetaIO-4.4.lib")
#pragma comment(lib, "ITKIOImageBase-4.4.lib")
#pragma comment(lib, "ITKIOTransformBase-4.4.lib")

ITKImageType::Pointer itk_create_image_3d(
  const ITKImageType::PixelType* ptr_pixel_data,
  const P3I& size,
  const P3D& origin,
  const P3D& spacing)
{
  auto result = ITKImageType::New();

  ITKImageType::PointType _origin;
  _origin[0] = origin.X(); // coordinates of the 
  _origin[1] = origin.Y(); // first pixel in N-D
  _origin[2] = origin.Z();
  result->SetOrigin(_origin);

  // Note: measurement units (e.g., mm, inches, etc.) are defined by the application.
  ITKImageType::SpacingType _spacing;
  _spacing[0] = spacing.X(); // spacing along X
  _spacing[1] = spacing.Y(); // spacing along Y
  _spacing[2] = spacing.Z(); // spacing along Z
  result->SetSpacing(_spacing);

  ITKImageType::IndexType _start;
  _start[0] = 0; // first index on X
  _start[1] = 0; // first index on Y
  _start[2] = 0; // first index on Z

  ITKImageType::SizeType _size;
  _size[0] = size.X(); // size along column
  _size[1] = size.Y(); // size along row
  _size[2] = size.Z(); // size along depth

  ITKImageType::RegionType region;
  region.SetIndex(_start);
  region.SetSize(_size);
  result->SetRegions(region);

  result->Allocate();

  int idx = 0;
  itk::ImageRegionIterator<ITKImageType> iter(result, region);
  while (!iter.IsAtEnd())
  {
    iter.Set(ptr_pixel_data[idx++]);
    ++iter;
  }

  return result;
}

void itk_save_image_3d(ITKImageType::Pointer image, std::string path)
{
  typedef itk::ImageFileWriter<ITKImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(path.c_str());
  writer->SetInput(image);
  writer->SetImageIO(itk::MetaImageIO::New());
  writer->Update();
}

void itk_save_image_3d(
  const ITKImageType::PixelType* ptr_pixel_data,
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

/*