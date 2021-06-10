// @refer vtk-conv-poly-data-to-image-data.cpp

// load/store image types

#pragma comment(lib, "vtkIOCore-9.0.lib")
#pragma comment(lib, "vtkIOLegacy-9.0.lib")

#include <vtkXMLPolyDataReader.h>
#pragma comment(lib, "vtkIOXML-9.0.lib")

#include <vtkXMLImageDataReader.h>
#include <vtkXMLImageDataWriter.h>

#include <vtkMarchingCubes.h>

// save vtkPolyData to .mhd file
{
  auto data = vtk_conv_poly_data_to_image_data(var_vtkPolyData);
  vtkNew<vtkMetaImageWriter> writer;
  writer->SetFileName("example.mhd");
  writer->SetInputData(data);
  writer->Write();
}

// save vtkPolyData to .vti file
{
  auto data = vtk_conv_poly_data_to_image_data(var_vtkPolyData);
  vtkNew<vtkXMLImageDataWriter> writer;
  writer->SetFileName("example.vti");
  writer->SetInputData(data.GetPointer());
  writer->Write();
}

// load vtkPolyData from .vti file
{
  file_path = file_dir + file_name;
  vtkNew<vtkXMLImageDataReader> reader;
  reader->SetFileName("example.vti");
  reader->Update();

  vtkNew<vtkMarchingCubes> surface; 
  surface->SetInputData(reader->GetOutput());
  surface->SetValue(0, 0.5);
  surface->Update();
  var_vtkPolyData = reader->GetOutput();
}

// save vtkPolyData to .vtk file
{
  vtkNew<vtkPolyDataWriter> writer;
  writer->SetFileName("example.vtk");
  writer->SetInputData(var_vtkPolyData);
  writer->Write();
}

// load vtkPolyData from .vtk file
{
  vtkNew<vtkPolyDataReader> reader;
  reader->SetFileName("example.vtk");
  reader->Update();
  var_vtkPolyData = reader->GetOutput();
};

// save vtkPolyData to .vtp file
{
  vtkNew<vtkXMLPolyDataWriter> writer;
  writer->SetFileName("example.vtp");
  writer->SetInputData(var_vtkPolyData);
  writer->SetDataModeToBinary(); // writer->SetDataModeToAscii();
  writer->Write();
}

// load vtkPolyData from .vtp file
{
  vtkNew<vtkXMLPolyDataReader> reader;
  reader->SetFileName(example.vtp);
  reader->Update();
  var_vtkPolyData = reader->GetOutput();
}
