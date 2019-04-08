
#include <DxLib.h>
#include "ImageMng.h"

std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::s_Instance(new ImageMng());

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}

const ImageName ImageMng::GetImage()
{
	return image;
}

const VEC_INT ImageMng::ImgGetID(std::string fileName)
{
	 // �摜ID�̃n���h����������Ȃ��������A�摜�ǂݍ��݂��s��(�����Ȃ�) 
	if (imgMap.find(fileName) == imgMap.end())
	{
		imgMap[fileName].resize(1);
		imgMap[fileName][0] = DxLib::LoadGraph(fileName.c_str(), true);
	}

	return imgMap[fileName];
}

const VEC_INT ImageMng::ImgGetID(std::string fileName, Vector2 divCnt, Vector2 divSize)
{
	 // �摜ID�̃n���h����������Ȃ��������A�摜�ǂݍ��݂��s��(��������) 
	if (imgMap.find(fileName) == imgMap.end())
	{
		imgMap[fileName].resize(divCnt.x * divCnt.y);
		DxLib::LoadDivGraph(fileName.c_str(), (divCnt.x * divCnt.y), divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap[fileName][0], true);
	}
	return imgMap[fileName];
}

