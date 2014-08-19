#include "CameraLink.h"
#include "QGLCanvas.h"

CameraLink::CameraLink( QGLCanvas* l, QGLCanvas* r )
: canvas_l(l), canvas_r(r)
{
	canvas_l->camera()->link(canvas_r->camera());
	canvas_l->addObserver(this);
	canvas_r->addObserver(this);
// 	std::set<const ObservableBase*> subjects = canvas_l->m_subjects;
// 	std::set<const ObservableBase*>::const_iterator iter;
// 	for (iter = subjects.begin(); iter != subjects.end(); ++ iter)
// 	{
// 		((Observable<IUpdatable>*)(*iter))->addObserver(this);
// 	}
// 	subjects = canvas_r->m_subjects;
// 	for (iter = subjects.begin(); iter != subjects.end(); ++ iter)
// 	{
// 		((Observable<IUpdatable>*)(*iter))->addObserver(this);
// 	}
}

CameraLink::~CameraLink(void)
{
	canvas_l->camera()->breakLink();
	canvas_l->removeObserver(this);
	canvas_r->removeObserver(this);

// 	std::set<const ObservableBase*> subjects = canvas_l->m_subjects;
// 	std::set<const ObservableBase*>::const_iterator iter = subjects.begin();
// 	for (; iter != subjects.end(); ++ iter)
// 	{
// 		((Observable<IUpdatable>*)(*iter))->removeObserver(this);
// 	}
// 	subjects = canvas_r->m_subjects;
// 	for (; iter != subjects.end(); ++ iter)
// 	{
// 		((Observable<IUpdatable>*)(*iter))->removeObserver(this);
// 	}
}

void CameraLink::udpate()
{
	canvas_l->update();
	canvas_r->update();
}
