num_feats = 4
num_levels_range = 2:4
num_r1 = 10000
num_r2 = 10000
prob_p = 5

outs = list()
for(a in 1:num_feats){
  nlev = sample(num_levels_range,1)
  prob = rev(as.vector(gtools::rdirichlet(1,seq_len(nlev)*50)))**prob_p
  prob = prob/sum(prob)
  matches = sample(nlev,num_r1*num_r2,prob = prob,replace=TRUE)-1
  matches_i = which(matches>0)
  rows = matches_i%%num_r1
  cols = floor(matches_i/num_r2)+1
  levels = matches[matches_i]
  df = data.frame(r=cols,c=rows,l=levels)
  write.table(df,paste0("rand_match_",a,"_",num_feats,".csv"),col.names = FALSE,row.names = FALSE)
  outs[[a]] = df
}
