#include <iostream>
#include <string>

class Args {
public:
	Args(int argc, char** argv) : m_Argc(argc), m_Argv(argv) {}

	char* operator[](int idx) const
	{
		if (idx < 0 || idx >= m_Argc)
			return nullptr;

		return m_Argv[idx];
	}

	int Size() const { return m_Argc; }

	bool HavePathFlag()
	{
		for (int i = 1; i != Size(); i++)
			if (!strcmp("--path", m_Argv[i]))
				return true;

		return false;
	}

	int PathFlagPosition()
	{
		for (int i = 1; i != Size(); i++)
			if (!strcmp("--path", m_Argv[i]))
				return i;

		return -1;
	}

	char* PathValue()
	{
		if (!HavePathFlag() || PathFlagPosition() == -1 || PathFlagPosition() >= m_Argc) return nullptr;

		return m_Argv[PathFlagPosition() + 1];
	}

private:
	int m_Argc;
	char** m_Argv;
};

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "CmdTools (tm) Developed by Milinkov Nikita (c) 2023\n" << std::endl;
		std::cout << "Usage:\tcmdtools add <name> --path <file>" << std::endl;
		return 0;
	}

	Args args(argc, argv);

	if (!strcmp("add", args[1]))
	{
		FILE* fp = { 0 };
		char filename[64] = { 0 };
		char* userprofile = { 0 };
		size_t sz = 0;

		if (args.Size() < 5)
		{
			std::cout << "cmdtools: not enough arguments." << std::endl;
			std::cout << "Usage:\tcmdtools add <name> --path <file>" << std::endl;
			return -1;
		}

		if (!strcmp("--path", args[2]))
		{
			std::cout << "cmdtools: expected alias name, found '--path' flag" << std::endl;
			std::cout << "Usage:\tcmdtools add <name> --path <file>" << std::endl;
			return -1;
		}

		if (_dupenv_s(&userprofile, &sz, "USERPROFILE") == 0 && userprofile != nullptr)
		{
			sprintf_s(filename, "%s\\Scripts\\%s.bat", userprofile, args[2]);

#ifdef _DEBUG
			std::cout << "[DEBUG] Final filename: " << filename << std::endl;
#endif

			fopen_s(&fp, filename, "w");
			if (fp != nullptr)
				fprintf_s(fp, "@echo off\n\"%s\" %%*", args.PathValue());

			free(userprofile);
		}
	}
	else
	{
		std::cout << "cmdtools: '" << args[1] << "' is not a cmdtools command." << std::endl;
		std::cout << "Usage:\tcmdtools add <name> --path <file>" << std::endl;
	}


	return 0;
}
