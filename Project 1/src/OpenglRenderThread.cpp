#include "OpenglRenderThread.h"
void OpenglRenderThread::run() {
	while (1) {
		emit nextFrame();
	}
}

void OpenglRenderThread::update() {
	emit nextFrame();
}
