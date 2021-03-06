#include "wav.h"
#include "metrics.h"

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
	const string name = R"(C:\TSvSI\coursework\resources\original_sound)";
	const string new_name = name + " modified";
	const string ext = ".wav";
	size_t message_len = 0;
	bool menu = true;
	
	while (menu) {
		system("cls");
		int choise;
		cout << "1. Write message to WAV file.\n"
		     << "2. Read message from WAV file.\n"
		     << "3. Evaluate MSE metric.\n"
		     << "4. Get info about WAV file.\n"
		     << "5. Exit.\n"
		     << "Make choise:";
		cin >> choise;
		
		switch (choise) {
			case 1: {
				system("cls");
				WAV wav_file;
				string message;
				std::ifstream input(name + ext, ios_base::in | ios_base::binary);
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
					system("pause");
					break;
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
				system("cls");
				WAV wav_file;
				ifstream input(new_name + ext, ios_base::in | ios_base::binary);
				wav_file.ReadHeader(input);
				wav_file.ReadWavData(input);
				wav_file.PrintWavInfo();
				cout << "Hide message: " << wav_file.GetMessageFromWAV(input, message_len) << "\n"
				     << "Press any key to continue...\n";
				system("pause");
				break;
			}
			case 3: {
				system("cls");
				WAV file;
				ifstream orig(name + ext, ios_base::binary | ios_base::in);
				ifstream modified(new_name + ext, ios_base::binary | ios_base::in);
				file.ReadHeader(orig);
				vector<char> orig_str(file.GetWavDataSize()), modified_str(file.GetWavDataSize());
				orig.read(orig_str.data(), file.GetWavDataSize());
				modified.ignore(44);
				modified.read(modified_str.data(), file.GetWavDataSize());
				cout << setprecision(4)
				<< "MSE: " << EvaluateMSE(orig_str, modified_str) << "\n"
				<< "NMSE: " << EvaluateNMSE(orig_str, modified_str) << "\n";
				orig.close();
				modified.close();
				system("pause");
				break;
			}
			case 4:{
				WAV file;
				ifstream input(name + ext, ios_base::binary | ios_base::in);
				file.ReadHeader(input);
				file.PrintWavInfo();
				input.close();
				system("pause");
				break;
			}
			case 5:{
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
