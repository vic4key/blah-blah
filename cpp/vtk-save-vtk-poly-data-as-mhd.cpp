// https://kitware.github.io/vtk-examples/site/Cxx/PolyData/PolyDataToImageData/

#include <vtkImageData.h>
#include <vtkImageStencil.h>
#include <vtkMetaImageWriter.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>

#pragma comment(lib, "vtkImagingStencil-9.0.lib")
#pragma comment(lib, "vtkCommonCore-9.0.lib")
#pragma comment(lib, "vtkFiltersSources-9.0.lib")
#pragma comment(lib, "vtkFiltersGeneral-9.0.lib")
#pragma comment(lib, "vtkCommonComputationalGeometry-9.0.lib")
#pragma comment(lib, "vtktiff-9.0.lib")
#pragma comment(lib, "vtkpugixml-9.0.lib")
#pragma comment(lib, "vtkCommonExecutionModel-9.0.lib")
#pragma comment(lib, "vtkjpeg-9.0.lib")
#pragma comment(lib, "vtkFiltersCore-9.0.lib")
#pragma comment(lib, "vtkpng-9.0.lib")
#pragma comment(lib, "vtkIOImage-9.0.lib")
#pragma comment(lib, "vtkCommonSystem-9.0.lib")
#pragma comment(lib, "vtksys-9.0.lib")
#pragma comment(lib, "vtkImagingCore-9.0.lib")
#pragma comment(lib, "vtkCommonMath-9.0.lib")
#pragma comment(lib, "vtkloguru-9.0.lib")
#pragma comment(lib, "vtkCommonTransforms-9.0.lib")
#pragma comment(lib, "vtkzlib-9.0.lib")
#pragma comment(lib, "vtkCommonDataModel-9.0.lib")
#pragma comment(lib, "vtkDICOMParser-9.0.lib")
#pragma comment(lib, "vtkCommonMisc-9.0.lib")
#pragma comment(lib, "vtkmetaio-9.0.lib")

// VTK 6.0
// #pragma comment(lib, "vtkIOImage-6.0.lib")
// #pragma comment(lib, "vtkImagingStencil-6.0.lib")

const auto fn_write_vtk_poly_data_as_mhd = [](vtkSmartPointer<vtkPolyData> pd, const std::string& file_name) -> void
{
  vtkNew<vtkImageData> whiteImage;
  double bounds[6];
  pd->GetBounds(bounds);
  double spacing[3]; // desired volume spacing
  spacing[0] = 0.5;
  spacing[1] = 0.5;
  spacing[2] = 0.5;
  whiteImage->SetSpacing(spacing);

  // compute dimensions
  int dim[3];
  for (int i = 0; i < 3; i++)
  {
    dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
  }
  whiteImage->SetDimensions(dim);
  whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);

  double origin[3];
  origin[0] = bounds[0] + spacing[0] / 2;
  origin[1] = bounds[2] + spacing[1] / 2;
  origin[2] = bounds[4] + spacing[2] / 2;
  whiteImage->SetOrigin(origin);
  whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

  // fill the image with foreground voxels:
  unsigned char inval  = 255;
  unsigned char outval = 0;
  vtkIdType count = whiteImage->GetNumberOfPoints();
  for (vtkIdType i = 0; i < count; ++i)
  {
    whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
  }

  // polygonal data --> image stencil:
  vtkNew<vtkPolyDataToImageStencil> pol2stenc;
  pol2stenc->SetInputData(pd);
  pol2stenc->SetOutputOrigin(origin);
  pol2stenc->SetOutputSpacing(spacing);
  pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
  pol2stenc->Update();

  // cut the corresponding white image and set the background:
  vtkNew<vtkImageStencil> imgstenc;
  imgstenc->SetInputData(whiteImage); // VTK 9.0
  // imgstenc->SetInputData(whiteImage.GetPointer()); // VTK 6.0
  imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
  imgstenc->ReverseStencilOff();
  imgstenc->SetBackgroundValue(outval);
  imgstenc->Update();

  vtkNew<vtkMetaImageWriter> writer;
  writer->SetFileName(file_name.c_str());
  writer->SetInputData(imgstenc->GetOutput());
  writer->Write();
};

fn_write_vtk_poly_data_as_mhd(var_vtk_poly_data, "test.mhd");