require 'set'

$board = []
(0..4).each{|y| (0..3).each{|x| $board << [x,y]}}

$init_state = {
  a1:[0,0],
  a2:[3,0],
  a3:[0,2],
  a4:[3,2],
  b:[1,2],
  c1:[0,4],
  c2:[1,3],
  c3:[2,3],
  c4:[3,4],
  d:[1,0],
}
$states = Set.new
$path = []

def finish?(state)
  state[:d]==[1,3]
end

def normalize(state)
  return {
    b:state[:b],
    d:state[:d],
    a:[state[:a1],state[:a2],state[:a3],state[:a4]].sort,
    c:[state[:c1],state[:c2],state[:c3],state[:c4]].sort,
  }
end

def eq_state(state1,state2)
  normalize(state1) == normalize(state2)
end

def dup_state(state1)
	$states.member?(normalize(state1))
end

def find_piece(state, cord)
  return nil if cord[0]<0 || cord[0]>3 || cord[1]<0 || cord[1]>4
  state.each do |k,v|
    return [k,v] if v==cord
    case k[0]
    when 'a'
      return [k,v] if [v[0],v[1]+1]==cord
    when 'b'
      return [k,v] if [v[0]+1,v[1]]==cord
    when 'd'
      return [k,v] if [v[0],v[1]+1]==cord
      return [k,v] if [v[0]+1,v[1]+1]==cord
      return [k,v] if [v[0]+1,v[1]]==cord
    end
  end
  :blank
end

def can_move?(state, type, cord, direction)
  case type[0]
  when 'a'
    case direction
    when 'left'
      return find_piece(state,[cord[0]-1,cord[1]])==:blank && find_piece(state,[cord[0]-1,cord[1]+1])==:blank
    when 'right'
      return find_piece(state,[cord[0]+1,cord[1]])==:blank && find_piece(state,[cord[0]+1,cord[1]+1])==:blank
    when 'up'
      return find_piece(state,[cord[0],cord[1]-1])==:blank
    when 'down'
      return find_piece(state,[cord[0],cord[1]+2])==:blank    
    else
      raise "illegal move #{direction}"
    end
  when 'b'
    case direction
    when 'left'
      return find_piece(state,[cord[0]-1,cord[1]])==:blank 
    when 'right'
      return find_piece(state,[cord[0]+2,cord[1]])==:blank
    when 'up'
      return find_piece(state,[cord[0],cord[1]-1])==:blank && find_piece(state,[cord[0]+1,cord[1]-1])==:blank
    when 'down'
      return find_piece(state,[cord[0],cord[1]+1])==:blank && find_piece(state,[cord[0]+1,cord[1]+1])==:blank  
    else
      raise "illegal move #{direction}"
    end
  when 'c'
    return true
  when 'd'
    case direction
    when 'left'
      return find_piece(state,[cord[0]-1,cord[1]])==:blank && find_piece(state,[cord[0]-1,cord[1]+1])==:blank
    when 'right'
      return find_piece(state,[cord[0]+2,cord[1]])==:blank && find_piece(state,[cord[0]+2,cord[1]+1])==:blank
    when 'up'
      return find_piece(state,[cord[0],cord[1]-1])==:blank && find_piece(state,[cord[0]+1,cord[1]-1])==:blank
    when 'down'
      return find_piece(state,[cord[0],cord[1]+2])==:blank && find_piece(state,[cord[0]+1,cord[1]+2])==:blank  
    else
      raise "illegal move #{direction}"
    end
  else
    raise "illegal type #{type}"
  end
end

def do_move(state, type, cord, direction)
  ret = state.dup
  case direction
    when 'left'
      ret[type] = [cord[0]-1,cord[1]]
    when 'right'
      ret[type] = [cord[0]+1,cord[1]]
    when 'up'
      ret[type] = [cord[0],cord[1]-1]
    when 'down'
      ret[type] = [cord[0],cord[1]+1]
    else
      raise "illegal move #{direction}"
  end
  ret
end

def all_moves_to(state, x1c)
  ret = []
  if(x1c[0]>0)
    cord = [x1c[0]-1,x1c[1]]
    piece = find_piece(state, cord)
    if piece && piece!=:blank && can_move?(state, piece[0], piece[1], 'right')
      ret << do_move(state, piece[0], piece[1], 'right')
    end
  end
  if(x1c[0]<3)
    cord = [x1c[0]+1,x1c[1]]
    piece = find_piece(state, cord)
    if piece && piece!=:blank && can_move?(state, piece[0], piece[1], 'left')
      ret << do_move(state, piece[0], piece[1], 'left')
    end
  end
  if(x1c[1]>0)
    cord = [x1c[0],x1c[1]-1]
    piece = find_piece(state, cord)
    if piece && piece!=:blank && can_move?(state, piece[0], piece[1], 'down')
      ret << do_move(state, piece[0], piece[1], 'down')
    end
  end
  if(x1c[1]<4)
    cord = [x1c[0],x1c[1]+1]
    piece = find_piece(state, cord)
    if piece && piece!=:blank && can_move?(state, piece[0], piece[1], 'up')
      ret << do_move(state, piece[0], piece[1], 'up')
    end
  end
  ret
end

def empty_cords(state)
  ret = $board.dup
  state.each do |k,v|
    ret.delete(v)
    case k[0]
    when 'a'
      ret.delete([v[0],v[1]+1])
    when 'b'
      ret.delete([v[0]+1,v[1]])
    when 'd'
      ret.delete([v[0],v[1]+1])
      ret.delete([v[0]+1,v[1]+1])
      ret.delete([v[0]+1,v[1]])
    end
  end
  raise "illegal state" if ret.size!=2
  ret
end

def moves(state)
  ret = []
  empty_cords(state).each{|cord| all_moves_to(state, cord).each{|y| ret << y} }
  ret
end

def print_state(state)
  arr = $board.map{|cord| find_piece(state, cord)}
  arr.each_with_index do |x,index|
    printf("\n") if index%4==0 && index>0
    if x != :blank
      printf("%-4s ",x[0])
    else
    	printf("%-4s", '--')
    end
  end
end

def search_recursive(state)
  #print_state(state)
  $path << state
  $states << normalize(state)
  arr = moves(state)
  #puts "valid move size: #{arr.size}"
  arr.each do |x|
    if finish?(x)
      $path << x
      puts $path
      return true
    end
  end
  arr.each do |x|
    search(x) unless dup_state(x)
  end
  $path = $path[0..-2]
end

def search(init)
	stack = [init]
	while !stack.empty?
    state = stack.delete_at(0)
    next if dup_state(state)
    #puts "\r\n\r\n"
    #print_state(state)
    $path << state
    $states << normalize(state)
    arr = moves(state)
    #puts "valid move size: #{arr.size}"
    arr.each do |x|
      if finish?(x)
        $path << x
        #puts $path
        print_state(x)
        puts "共#{$path.size}步"
        return true
      end
    end
    arr.each{|x| stack << x}
  end
end

search($init_state)
