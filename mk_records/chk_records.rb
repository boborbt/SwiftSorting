require 'set'


records_count = 0
strings = Set.new
ints = Set.new
floats = Set.new

File.open("records.csv") do |file|
  file.each do |line|
    record = line.split(',')

    if record.size != 4
      puts "record #{record} has less than 4 fields"
      exit(1)
    end

    if records_count % 200_000 == 0
      puts "Progress: %4.2f" % [records_count.to_f / 200_000]
    end

    records_count += 1
    strings.add(record[1])
    ints.add(record[2])
    floats.add(record[3])
  end
end

puts "Num records: #{records_count}"
puts "Num strings: #{strings.size}"
puts "Num integers: #{ints.size}"
puts "Num floats: #{floats.size}"
