/*
 * Proccessing Seismic Events
 * By Amirhossein Naemi
 * Febuary, 2015
 * GIT: https://github.com/amirhossein-naemi/Seismic-Events
 */

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const int max_stations    = 100;
const int max_instruments = 10;

const int max_st_size  = 10;
const int max_ins_size = 15;

bool read_input(string inputfilename, int arr[max_stations][max_instruments],
                string stations_name[max_stations],
                string instruments_name[max_instruments], int & number_of_stations,
                int & number_of_instruments) {
    
    ifstream ifile(inputfilename.c_str());
    int i, tmp_v;
    int entry     = 0;
    int entry_err = 0;
    string tmp_st, tmp_ins;
    bool err_station, err_instrument, err_negative_event;
    
    if (!ifile.is_open())
        return false;
    
    ofstream oerrfile("amir.err");
    if (!oerrfile.is_open())
        return false;
    
    ifile >> number_of_stations;
    ifile >> number_of_instruments;
    
    oerrfile << "Number of stations: "    << number_of_stations    << endl;
    oerrfile << "Number of instruments: " << number_of_instruments << endl;
    
    for (i = 0; i < number_of_stations; i++)
        ifile >> stations_name[i];
    oerrfile << "Stations read" << endl;
    
    for (i = 0; i < number_of_instruments; i++)
        ifile >> instruments_name[i];
    oerrfile << "Instruments read" << endl;
    
    oerrfile << "Reading entries ..." << endl;
    
    while (ifile >> tmp_st) {
        
        entry++;
        
        ifile >> tmp_ins;
        ifile >> tmp_v;
        
        int idx_station    = 0;
        int idx_instrument = 0;
        
        // if no error found, it replaces the true with false
        err_station = true;
        err_instrument = true;
        err_negative_event = true;
        
        for (i = 0; i < number_of_stations; i++)
            if (tmp_st == stations_name[i]) {
                idx_station = i;
                err_station = false;
            }
        
        if (err_station)
            oerrfile << "Entry #" << entry << ": " << " " << setw(max_st_size)
            << tmp_st << " " << setw(max_ins_size) << tmp_ins << " " << setw
            (max_st_size)
            << tmp_v << " -- Warning: Station does not exist" << endl;
        
        for (i = 0; i < number_of_instruments; i++)
            if (tmp_ins == instruments_name[i]) {
                idx_instrument = i;
                err_instrument = false;
            }
        
        if (err_instrument)
            oerrfile << "Entry #" << entry << ": " << " " << setw(max_st_size)
            << tmp_st << " " << setw(max_ins_size) << tmp_ins << " " << setw
            (max_st_size)
            << tmp_v << " -- Warning: Instrument does not exist" << endl;
        
        if (tmp_v >= 0)
            err_negative_event = false;
        
        if (err_negative_event)
            oerrfile << "Entry #" << entry << ": " << " " << setw(max_st_size)
            << tmp_st << " " << setw(max_ins_size) << tmp_ins << " " << setw
            (max_st_size)
            << tmp_v << " -- Warning: Negative events" << endl;
        
        if (!err_station && !err_instrument && !err_negative_event)
            arr[idx_station][idx_instrument] += tmp_v;
        else
            entry_err++;
        
    }
    
    oerrfile << "Total of " << (entry - entry_err)
    << " entries processed correctly" << endl;
    
    oerrfile << "Total of " << entry_err << " entries ignored" << endl;
    
    oerrfile << "Generating report..." << endl;
    
    return true;
}

int max(int A[], int number_of_elements) {
    
    int maxval = A[0];
    for (int i = 0; i < number_of_elements; i++)
        if (A[i] > maxval)
            maxval = A[i];
    
    return maxval;
}

int min(int A[], int number_of_elements) {
    
    int minval = 99999999; // largest possible number of events
    for (int i = 0; i < number_of_elements; i++)
        if (A[i] < minval)
            minval = A[i];
    
    return minval;
}

// Processing the read data from input file
bool write_output(int arr[max_stations][max_instruments],
                  string stations_name[max_stations],
                  string instruments_name[max_instruments], int & number_of_stations,
                  int & number_of_instruments) {
    
    int sum_per_station[max_stations] = {0};
    int sum_per_instruments[max_instruments] = {0};
    int i, j;
    
    ofstream ofile("amir.out");
    if (!ofile.is_open())
        return false; // indicate something wrong...
    
    ofile << "Seismic Events Summary Report" << endl;
    cout << "Seismic Events Summary Report" << endl;
    ofile << setiosflags(ios::fixed) << left << setw(max_st_size) << "Station";
    cout << setiosflags(ios::fixed) << left << setw(max_st_size) << "Station";
    
    for (j = 0; j < number_of_instruments; j++) {
        ofile << "|" << setw(max_ins_size) << left << instruments_name[j];
        cout << "|" << setw(max_ins_size) << left << instruments_name[j];
    }
    
    ofile << endl;
    cout << endl;
    
    for (i = 0; i < number_of_stations; i++) {
        
        ofile << left << setw(max_st_size) << setfill(' ') << stations_name[i];
        
        for (j = 0; j < number_of_instruments; j++) {
            ofile << "|" << setw(max_ins_size) << left << arr[i][j];
        }
        
        ofile << endl;
        
        cout << left << setw(max_st_size) << setfill(' ') << stations_name[i];
        
        for (j = 0; j < number_of_instruments; j++) {
            cout << "|" << setw(max_ins_size) << left << arr[i][j];
        }
        
        cout << endl;
    }
    
    ofile << endl;
    cout  << endl;
    
    ofile << "Total number of events per Station" << endl;
    ofile << left << setw(max_st_size) << "Station" << "|" << setw(max_st_size)
    << "Total" << endl;
    
    cout << "Total number of events per Station" << endl;
    cout << left << setw(max_st_size) << "Station" << "|" << setw(max_st_size)
    << "Total" << endl;
    
    for (i = 0; i < number_of_stations; i++) {
        
        for (j = 0; j < number_of_instruments; j++)
            sum_per_station[i] += arr[i][j];
        
        ofile << left << setw(max_st_size) << setfill(' ') << stations_name[i];
        ofile << left << "|" << setw(max_st_size) << setfill(' ')
        << sum_per_station[i] << endl;
        
        cout << left << setw(max_st_size) << setfill(' ') << stations_name[i];
        cout << left << "|" << setw(max_st_size) << setfill(' ')
        << sum_per_station[i] << endl;
    }
    
    ofile << endl;
    
    ofile << "Total number of events per instrument" << endl;
    ofile << left << setw(max_ins_size) << "Instrument" << "|" << setw
    (max_st_size) << "Total" << endl;
    
    cout << "Total number of events per instrument" << endl;
    cout << left << setw(max_ins_size) << "Instrument" << "|" << setw
    (max_st_size) << "Total" << endl;
    
    for (j = 0; j < number_of_instruments; j++) {
        
        for (i = 0; i < number_of_stations; i++) {
            sum_per_instruments[j] += arr[i][j];
        }
        
        ofile << left << setw(max_ins_size) << setfill(' ')
        << instruments_name[j];
        
        ofile << left << "|" << setw(max_ins_size) << setfill(' ')
        << sum_per_instruments[j] << endl;
        
        cout << left << setw(max_ins_size) << setfill(' ')
        << instruments_name[j];
        
        cout << left << "|" << setw(max_ins_size) << setfill(' ')
        << sum_per_instruments[j] << endl;
    }
    
    ofile << endl;
    
    int most_records_station = max(sum_per_station, number_of_stations);
    int most_records_instrument =
    max(sum_per_instruments, number_of_instruments);
    
    int least_records_station = min(sum_per_station, number_of_stations);
    int least_records_instrument =
    min(sum_per_instruments, number_of_instruments);
    
    for (i = 0; i < number_of_stations; i++) {
        
        if (sum_per_station[i] == most_records_station)
            ofile << "Station with the most records is: " << stations_name[i]
            << " with " << sum_per_station[i] << " events" << endl;
        
        if (sum_per_station[i] == least_records_station)
            ofile << "Station with the least records is: " << stations_name[i]
            << " with " << sum_per_station[i] << " events" << endl;
    }
    
    for (i = 0; i < number_of_instruments; i++) {
        
        if (sum_per_instruments[i] == most_records_instrument)
            ofile << "Instrument with the most records is: " << instruments_name
            [i] << " with " << sum_per_instruments[i] << " events" << endl;
        
        if (sum_per_instruments[i] == least_records_instrument)
            ofile << "Instrument with the least records is: " <<
            instruments_name[i] << " with " << sum_per_instruments[i]
            << " events" << endl;
    }
    
    cout << "Wrote output to file: " << "amir.out" << endl;
    
    return true;
}

int main() {
    
    string infilename;
    
    int results[max_stations][max_instruments] = {0};
    string stations_name[max_stations];
    string instruments_name[max_instruments];
    
    int number_of_stations, number_of_instruments;
    
    cout << "Enter input file: ";
    cin  >> infilename;

    // for testing purposes:
    //infilename = "c:\\Input.txt"; // windows
    //infilename="/Users/Amir/Desktop/Input.txt"; // unix
    
    cout << "Reading input from input file: " << infilename << endl;
    if (!read_input(infilename, results, stations_name, instruments_name,
                    number_of_stations, number_of_instruments)) {
        cout << "Error in reading the file " << infilename << endl;
        return 1;
    }
    
    cout << "Writing output to file: " << "amir.out" << endl;
    if (!write_output(results, stations_name, instruments_name,
                      number_of_stations, number_of_instruments)) {
        cout << "Problems writing output file amir.out";
        return 2;
    }
    
    return 0;
}
