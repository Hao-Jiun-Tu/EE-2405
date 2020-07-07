# Part. 1

#=======================================

# Import module

#  csv -- fileIO operation

import csv
import collections
#=======================================


# Part. 2

#=======================================

# Read cwb weather data

cwb_filename = '107060011.csv'

data = []

header = []

with open(cwb_filename) as csvfile:

   mycsv = csv.DictReader(csvfile)

   header = mycsv.fieldnames

   for row in mycsv:

      data.append(row)

#=======================================


# Part. 3

#=======================================

# Analyze data depend on your group and store it to target_data like:

# Retrive all data points which station id is "C0X260" as a list.

#del data['obsTime']

target_data0 = list(filter(lambda item: item['station_id'] == 'C0A880', data))
target_data1 = list(filter(lambda item: item['station_id'] == 'C0F9A0', data))
target_data2 = list(filter(lambda item: item['station_id'] == 'C0G640', data))
target_data3 = list(filter(lambda item: item['station_id'] == 'C0R190', data))
target_data4 = list(filter(lambda item: item['station_id'] == 'C0X260', data))

# Retrive ten data points from the beginning.

target = []

n = 0
data0 = []
while n < len(target_data0) :
   data0.append(target_data0[n]['TEMP'])
   n += 1

data0.sort();
if data0[-1] == '-99.000' or data0[-1] == '-999.000':
   target.append(['C0A880','None'])
else :
   target.append(['C0A880',data0[-1]])
#print (data0)

n = 0
data1 = []
while n < len(target_data1) :
   data1.append(target_data1[n]['TEMP'])
   n += 1

data1.sort();
if data1[-1] == '-99.000' or data1[-1] == '-999.000':
   target.append(['C0F9A0','None'])
else :
   target.append(['C0F9A0',data1[-1]])
#print (data1)

n = 0
data2 = []
while n < len(target_data2) :
   data2.append(target_data2[n]['TEMP'])
   n += 1

data2.sort();
if data2[-1] == '-99.000' or data2[-1] == '-999.000':
   target.append(['C0G640','None'])
else :
   target.append(['C0G640',data2[-1]])
#print (data2)

n = 0
data3 = []
while n < len(target_data3) :
   data3.append(target_data3[n]['TEMP'])
   n += 1

data3.sort();
if data3[-1] == '-99.000' or data3[-1] == '-999.000':
   target.append(['C0R190','None'])
else :
   target.append(['C0R190',data3[-1]])
#print (data3)

n = 0
data4 = []
while n < len(target_data4) :
   data4.append(target_data4[n]['TEMP'])
   n += 1

data4.sort();
if data4[-1] == '-99.000' or data4[-1] == '-999.000':
   target.append(['C0X260','None'])
else :
   target.append(['C0X260',data4[-1]])
#print (data4)

#=======================================


# Part. 4

#=======================================

# Print result

print(target)

#========================================


