from PyVutils import Cv

def fn_callback(frame, frameinfo, *args):
	Cv.DrawText(frame, 40, 40, f"{frameinfo}")
	frame = Cv.Invert(frame)
	return frame

Cv.Camera(fn_callback)