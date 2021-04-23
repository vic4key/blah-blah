from PyVutils import Cv

video_file_path = R"data\ExpressKeys.mp4"

def fn_callback(frame, frameinfo, *args):
	Cv.DrawText(frame, 40, 40, f"{frameinfo}")
	frame = Cv.Invert(frame)
	return frame

Cv.Video(video_file_path, fn_callback)