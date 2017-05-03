# Builds a dataset to test sorting algorithms.
# Fields:
#     id: integer
# field1: string              random strings, high number of repetitions
# field2: integer             random integers, low number of repetitions
# field3: floating point      random floats, almost no repetition
#
# This program generates 20 milion records

NUM_RECORDS = 20_000_000
PROGRESS_FREQUENCY = 200_000

class RandomGenerator
  STRING_POOL_SIZE = 1000        # about 20_000 repetitions per string
  INTEGER_POOL_SIZE =5_000_000  # about 4 repetition per integer

  def initialize
    @generator = Random.new
    @string_pool = generate_strings(STRING_POOL_SIZE)
  end

  def generate_strings(count)
    words = File.read("divina_commedia.txt").split(/[\n \t,«]/).reject { |x| x.empty? || x.size <= 3 }
    words.sample(count)
  end

  def random_string(i)
    @string_pool[@generator.rand(STRING_POOL_SIZE)]
  end

  def random_integer(i)
    @generator.rand(INTEGER_POOL_SIZE)
  end

  def random_float(i)
    @generator.rand(100000.0)
  end

end

File.open('records.csv', 'w') do |file|
  rg = RandomGenerator.new

  NUM_RECORDS.times do |i|
    if i % PROGRESS_FREQUENCY == 0
      puts "Progress: %4.2f%" % [100.0 * i.to_f / NUM_RECORDS]
    end

    file.puts "%d,%s,%d,  %12.6f" % [i, rg.random_string(i), rg.random_integer(i), rg.random_float(i)]
  end
end
