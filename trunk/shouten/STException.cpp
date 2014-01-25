#include "STException.h"

using namespace ST;

Exception::Exception(const String& content, const String& type):
	mContent(content), mType(type)
{

}

Exception::~Exception()
{}