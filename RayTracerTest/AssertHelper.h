#pragma once
#include "../Vector.h"
#include "CppUnitTest.h"

inline void AssertAreEqual(Vector3 expected, Vector3 actual, double epsilon, const wchar_t* message = NULL)
{
	bool equals = expected.equals(actual, epsilon);
	std::ostringstream s;
	if (message)
	{
		s << message << std::endl;
	}
	s << "Expected: " << expected << " Actual: " << actual << std::endl;
	
	std::string logMessage = s.str();
	std::wstring wMessage(logMessage.begin(), logMessage.end());
	Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(equals, wMessage.c_str());
}