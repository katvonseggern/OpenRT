#include "openrt.h"
#include "core/timer.h"

using namespace rt;

int main(int argc, char* argv[])
{
	const Size resolution(1000, 600);

	CScene scene(RGB(0.4f, 0.4f, 0.4f));

	auto pShaderTop  	= std::make_shared<CShaderPhong>(scene, RGB(0.90f, 0.75f, 0.70f), 0.5f, 0.5f, 0.0f, 40);
	auto pShaderSide 	= std::make_shared<CShaderPhong>(scene, RGB(0.55f, 0.65f, 0.70f), 0.7f, 0.5f, 0.0f, 40);
	auto pShaderWhite	= std::make_shared<CShaderFlat>(Vec3f::all(1));
	auto pShaderFloor	= std::make_shared<CShaderPhong>(scene, RGB(1, 1, 1), 0.5f, 0.5f, 0.0f, 40);
	auto pShaderGlass	= std::make_shared<CShader>(scene, RGB(0.55f, 0.65f, 0.70f), 0, 0.1f, 2, 80, 0.2f, 0.8f, 1.5f);

	//CSolid torus(pShaderTop, "../../../data/Torus Knot.obj"); // "D:\\Projects\\OpenRT\\data\\Torus Knot.obj");

	// primitives
	//scene.add(std::make_shared<CPrimPlane>(pShaderFloor, Vec3f(0, 0, 0), Vec3f(0, 1, 0)));
	const float s = 50;
	const float h = 0;
	CSolidQuad quad(pShaderFloor, Vec3f(-s, h, -s), Vec3f(-s, h, s), Vec3f(s, h, s), Vec3f(s, h, -s));
	CSolidBox cube(pShaderGlass, Vec3f(0, 1.01f, 0), 0.1f, 2, 6);
	scene.add(quad);
	scene.add(cube);
	//scene.add(torus);
	
	scene.add(std::make_shared<CPrimTriangle>(pShaderWhite, Vec3f(-10, 10.01f, -10), Vec3f(-10, 10.01f, 10), Vec3f(10, 10.01f, -10)));
	scene.add(std::make_shared<CPrimTriangle>(pShaderWhite, Vec3f(10, 10.01f, 10), Vec3f(-10, 10.01f, 10), Vec3f(10, 10.01f, -10)));
	
	scene.add(std::make_shared<CPrimSphere>(pShaderTop, Vec3f(-2, 1, 2), 1));
	//scene.add(std::make_shared<CPrimSphere>(std::make_shared<CShader>(scene, RGB(0, 0.5f, 1), 0, 0, 1, 80, 0.2f, 0.8f, 2.4f, std::make_shared<CSamplerRandom>(3, true, 0.1f)), Vec3f(-2, 1, 2), 1));
	//scene.add(std::make_shared<CPrimSphere>(std::make_shared<CShader>(scene, 0.35f, 0.65f, 1.517), Vec3f(2, 1, -2), 1));
	
	scene.add(std::make_shared<CPrimSphere>(pShaderSide, Vec3f(2, 1, -2), 1));


	// lights
	//scene.add(std::make_shared<CLightPoint>(Vec3f(50, 50, 50), Vec3f(-10, 10, -10), true));
	//scene.add(std::make_shared<CLightPoint>(Vec3f(50, 50, 50), Vec3f(-10, 10, 10), true));
	//scene.add(std::make_shared<CLightPoint>(Vec3f(50, 50, 50), Vec3f(10, 10, 10), true));
	//scene.add(std::make_shared<CLightPoint>(Vec3f(50, 50, 50), Vec3f(10, 10, -10), true));
	scene.add(std::make_shared<CLightPoint>(Vec3f::all(50), Vec3f(0, 1, 10), false));
	scene.add(std::make_shared<CLightArea>(Vec3f::all(6), Vec3f(-10, 10, -10), Vec3f(10, 10, -10), Vec3f(10, 10, 10), Vec3f(-10, 10, 10), std::make_shared<CSamplerStratified>(4, true, true)));


	// camera
	const float r = 17.5f;
	Vec3f camPos(sqrt(r), sqrt(r), sqrt(r));
	scene.add(std::make_shared<CCameraPerspective>(camPos, normalize(Vec3f(0, 0.5f, 0) - camPos), Vec3f(0, 1, 0), 45, resolution));
	
#ifdef ENABLE_BSP
	scene.buildAccelStructure();
#endif

	// render three images with different camera settings
	Timer::start("Rendering... ");
	Mat img = scene.render(std::make_shared<CSamplerStratified>(4, false));
	Mat depth = scene.renderDepth();
	Timer::stop();
	
	imshow("Image", img);
	imwrite("cube.jpg", img);
	
	depth.convertTo(depth, CV_8UC1, 10);
	imshow("Depth", depth);
	imwrite("depth.jpg", depth);
	
	waitKey();
	return 0;
}


