
-- function comp_nand(name,params)
--     input1=params[3]
--     input2=params[2]
--     output=params[1]
--     return {
--         {name,input1,input2,-output},
--         {name,input1,-input2,output},
--         {name,-input1,input2,output},
--         {name,-input1,-input2,-output}
--     }
-- end

function comp_and(name,params)
    result = {}
    output=params[1]
    long_clause= {name}
    count=0
    for key,value in ipairs(params)
    do
        count=count+1
        if(key>1)
        then
            result[key]={name,value,-output}
            long_clause[key]=-value
        end
    end
    long_clause[count+1]=output
    result[1]=long_clause
    return result
end

function comp_nand(name,params)
    result = {}
    output=params[1]
    long_clause= {name}
    count=0
    for key,value in ipairs(params)
    do
        count=count+1
        if(key>1)
        then
            result[key]={name,value,output}
            long_clause[key]=-value
        end
    end
    long_clause[count+1]=-output
    result[1]=long_clause
    return result
end

function comp_not(name,params)
    input=params[2]
    output=params[1]
    return {
        {name,input,output},
        {name,-input,-output},
     }
 end

 function comp_buf(name,params)
     input=params[2]
     output=params[1]
     return {
         {name,input,-output},
         {name,-input,output},
      }
  end

 function comp_or(name,params)
     result = {}
     output=params[1]
     long_clause= {name}
     count=0
     for key,value in ipairs(params)
     do
         count=count+1
         if(key>1)
         then
             result[key]={name,-value,output}
             long_clause[key]=value
         end
     end
     long_clause[count+1]=-output
     result[1]=long_clause
     return result
 end

  function comp_nor(name,params)
      result = {}
      output=params[1]
      long_clause= {name}
      count=0
      for key,value in ipairs(params)
      do
          count=count+1
          if(key>1)
          then
              result[key]={name,-value,-output}
              long_clause[key]=value
          end
      end
      long_clause[count+1]=output
      result[1]=long_clause
      return result
  end

function comp_xor(name,params)
    input1=params[3]
    input2=params[2]
    output=params[1]
    return {
        {name,input1,input2,output},
        {name,input1,-input2,-output},
        {name,-input1,input2,-output},
        {name,-input1,-input2,output}
    }
end