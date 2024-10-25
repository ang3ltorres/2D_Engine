#include "texture.hpp"
#include "graphics.hpp"
#include "utils.hpp"

#include <wincodec.h>

Texture::Texture(const std::string &fileName)
{
	IWICImagingFactory *wicFactory = nullptr;
	CoCreateInstance
	(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory
	);

	IWICBitmapDecoder *wicDecoder = nullptr;
	wicFactory->CreateDecoderFromFilename
	(
		toWideString(fileName).c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder
	);

	IWICBitmapFrameDecode *wicFrame = nullptr;
	wicDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter *wicConverter = nullptr;
	wicFactory->CreateFormatConverter(&wicConverter);

	wicConverter->Initialize
	(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	Graphics::render->CreateBitmapFromWicBitmap
	(
		wicConverter,
		nullptr,
		&bitmap
	);

	// Get texture size
	auto textureSize = bitmap->GetPixelSize();
	width = textureSize.width;
	height = textureSize.height;

	// Print message
	// std::printf("Texture: \"{}\" loaded\nWidth: {}\nHeight: {}\n", std::filesystem::path(fileName).filename().string(), width, height);

	wicConverter->Release();
	wicFrame->Release();
	wicDecoder->Release();
	wicFactory->Release();
}

Texture::Texture(const Texture &other)
{
	const D2D1_SIZE_U size = other.bitmap->GetPixelSize();
	const D2D1_BITMAP_PROPERTIES props =
	{
		.pixelFormat =
		{
			.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		.dpiX = 0.0f,
		.dpiY = 0.0f
	};

	Graphics::render->CreateBitmap(size, nullptr, 0, &props, &bitmap);

	const D2D1_POINT_2U destination = {0, 0};
	const D2D1_RECT_U source = {0, 0, size.width, size.height};
	bitmap->CopyFromBitmap(&destination, other.bitmap, &source);
}

Texture::~Texture()
{
	bitmap->Release();
}
