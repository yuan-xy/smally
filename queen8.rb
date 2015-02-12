def feasible0(arr, cord)
  arr.each do | oldc|
   return false if oldc[0] == cord[0] || oldc[1] == cord[1]
   return false if (oldc[0]-cord[0]) == (oldc[1] - cord[1])
   return false if (oldc[0]-cord[0]) == -(oldc[1] - cord[1])
  end
  return true
end

def feasible(arr)
  feasible0(arr[0..-2],arr[-1])
end


def solve
  count =0
  (1..8).each do |x1|
    p1 = [1, x1]
    (1..8).each do |x2|
      p2 = [2, x2]
      next unless feasible([p1, p2]) 
      (1..8).each do |x3|
        p3 = [3, x3]
        next unless feasible([p1, p2, p3])   
        (1..8).each do |x4|
          p4 = [4, x4]
          next unless feasible([p1, p2, p3, p4])   
          (1..8).each do |x5|
            p5 = [5, x5]
            next unless feasible([p1, p2, p3, p4, p5]) 
            (1..8).each do |x6|
              p6 = [6, x6]
              next unless feasible([p1, p2, p3, p4, p5, p6]) 
              (1..8).each do |x7|
                p7 = [7, x7]
                next unless feasible([p1, p2, p3, p4, p5, p6, p7]) 
                (1..8).each do |x8|
                  p8 = [8, x8]
                  solution = [p1, p2, p3, p4, p5, p6, p7, p8]
                	if feasible(solution) 
                	  count += 1
                	  puts "solution: #{solution}"
                	end
                end #8
              end #7
            end #6
          end #5
        end #4
      end #3
    end #2
  end #1
  puts "all count: #{count}"
end

solve

