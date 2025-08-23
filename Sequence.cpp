#include "Sequence.h"
#include "Title.h"
#include <vector>
#include <algorithm>

Sequence::~Sequence()
{
	while (!mActors.empty()) {
		delete mActors.back();
		mActors.pop_back();
	}
}

void Sequence::input()
{
}

void Sequence::update()
{
}

void Sequence::draw()
{
}

void Sequence::unloadData()
{
	for (auto tex : mTextures)
	{
		UnloadTexture(tex.second);
	}
}

void Sequence::addActor(Actor* actor)
{
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Sequence::removeActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

Texture2D Sequence::getTexture(const std::string& fileName)
{
	Texture2D tex;
	// テクスチャが既に連想配列に入っているかチェック
	auto iter = mTextures.find(fileName);
	// ロード済み
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	// 未ロード
	else {
		tex = LoadTexture(fileName.c_str());
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
