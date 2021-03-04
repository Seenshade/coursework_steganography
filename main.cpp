#include "wav.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	const string name = R"(C:\TSvSI\coursework\original_sound\resources)";
	const string new_name = name + " modified";
	const string ext = ".wav";
	size_t message_len = 0;
	bool menu = true;
	
	while (menu) {
		int choise;
		cout << "1. Write message to WAV file.\n"
		     << "2. Read message from WAV file.\n"
		     << "3. Exit.\n"
		     << "Make choise:";
		cin >> choise;
		
		switch (choise) {
			case 1: {
				WAV wav_file;
				string message;
				std::ifstream input;
				input.open(name + ext, ios_base::in | ios_base::binary);
				wav_file.ReadHeader(input);
				wav_file.ReadWavData(input);
				wav_file.PrintWavInfo();
				input.close();
				cout << "Write your message:\n";
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				getline(cin, message);
				message_len = message.length();
				try {
					wav_file.WriteMessageToWAV(message);
				} catch (invalid_argument ex) {
					cerr << ex.what();
				}
				ofstream output;
				output.open(new_name + ext, ios_base::out | ios_base::binary);
				wav_file.WriteModified(output);
				output.close();
				cout << "Message insert successful\n"
				     << "Press any key to continue...\n";
				system("pause");
				break;
			}
			case 2: {
				WAV wav_file;
				ifstream input;
				input.open(new_name + ext, ios_base::in | ios_base::binary);
				wav_file.ReadHeader(input);
				wav_file.ReadWavData(input);
				wav_file.PrintWavInfo();
				cout << "Hide message: " << wav_file.GetMessageFromWAV(input, message_len) << "\n"
				     << "Press any key to continue...\n";
				system("pause");
				break;
			}
			case 3: {
				menu = false;
				break;
			}
			default: {
				cout << "Wrong choise. Try again.\n";
				break;
			}
		}
	}
	return 0;
}
