#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkDataSetMapper.h>
#include <vtkXMLImageDataReader.h>
#include <vtkRenderWindowInteractor.h>
#ifdef _DEBUG
#pragma comment(lib, "vtkInfovisCore-9.0d.lib")
#pragma comment(lib, "vtkCommonColor-9.0d.lib")
#pragma comment(lib, "vtkRenderingCore-9.0d")
#pragma comment(lib, "vtkInteractionWidgets-9.0d")
#pragma comment(lib, "vtkInteractionStyle-9.0d.lib")
#pragma comment(lib, "vtkRenderingFreeType-9.0d.lib")
#pragma comment(lib, "vtkRenderingOpenGL2-9.0d.lib")
#else
#pragma comment(lib, "vtkInfovisCore-9.0.lib")
#pragma comment(lib, "vtkCommonColor-9.0.lib")
#pragma comment(lib, "vtkRenderingCore-9.0.lib")
#pragma comment(lib, "vtkInteractionWidgets-9.0.lib")
#pragma comment(lib, "vtkInteractionStyle-9.0.lib")
#pragma comment(lib, "vtkRenderingFreeType-9.0.lib")
#pragma comment(lib, "vtkRenderingOpenGL2-9.0.lib")
#endif // DEBUG

#include <vtkAutoInit.h> 
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)

static void vtk_visualize_poly_data(const tkSmartPointer<vtkImageData>& data)
{
  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputData(data);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetRepresentationToWireframe();

  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  renderer->ResetCamera();
  renderer->SetBackground(colors->GetColor3d("Silver").GetData());

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("ReadImageData");

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindow->Render();
  renderWindowInteractor->Initialize();

  renderWindowInteractor->Start();
}
