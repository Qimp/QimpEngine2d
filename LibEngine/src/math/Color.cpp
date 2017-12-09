#include "Private.h"


QIMP_BEGIN_NAMESPACE

const Colorf Colorf::Zero = Colorf(0, 0, 0, 0);
const Colorf Colorf::Black = Colorf(0, 0, 0, 1.0f);
const Colorf Colorf::Red = Colorf(1.0f, 0, 0, 1.0f);
const Colorf Colorf::Green = Colorf(0, 1.0f, 0, 1.0f);
const Colorf Colorf::Blue = Colorf(0, 0, 1.0f, 1.0f);
const Colorf Colorf::Yellow = Colorf(1.0f, 1.0f, 0, 1.0f);
const Colorf Colorf::Magenta = Colorf(1.0f, 0, 1.0f, 1.0f);
const Colorf Colorf::Cyan = Colorf(0, 1.0f, 1.0f, 1.0f);
const Colorf Colorf::White = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
const Colorf Colorf::LtGrey = Colorf(0.75f, 0.75f, 0.75f, 1.0f);
const Colorf Colorf::MdGrey = Colorf(0.5f, 0.5f, 0.5f, 1.0f);
const Colorf Colorf::DkGrey = Colorf(0.247f, 0.247f, 0.247f, 1.0f);

const Color32 Color32::Zero = Color32(0, 0, 0, 0);
const Color32 Color32::Black = Color32(0, 0, 0, 255);
const Color32 Color32::Red = Color32(255, 0, 0, 255);
const Color32 Color32::Green = Color32(0, 255, 0, 255);
const Color32 Color32::Blue = Color32(0, 0, 255, 255);
const Color32 Color32::Yellow = Color32(255, 255, 0, 255);
const Color32 Color32::Magenta = Color32(255, 0, 255, 255);
const Color32 Color32::Cyan = Color32(0, 255, 255, 255);
const Color32 Color32::White = Color32(255, 255, 255, 255);
const Color32 Color32::LtGrey = Color32(191, 191, 191, 255);
const Color32 Color32::MdGrey = Color32(127, 127, 127, 255);
const Color32 Color32::DkGrey = Color32(63, 63, 63, 255);

QIMP_END_NAMESPACE
