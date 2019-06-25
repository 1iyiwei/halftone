#!/bin/tcsh -f

set blue_dir = ../../BlueNoise/src;
set rainbow_dir = ../../RainbowNoise/src;
set pdm_dir = ../../PoissonTextureMap/src;
set dst_dir = .;

foreach file () #(Library/MD)
svn cp ${blue_dir}/${file} ${dst_dir}/${file};
end

foreach file () #(Library/rand48 Library/Library.vcproj)
svn cp ${rainbow_dir}/${file} ${dst_dir}/${file};
end

foreach class () #(Library/NBallSliceCounter Library/OnionLayerCounter Library/RandomCounter) 
foreach ext (hpp cpp)

svn cp ${blue_dir}/${class}.${ext} ${dst_dir}/${class}.${ext};

end
end

foreach class () #(Library/Counter Library/SequentialCounter Library/Math Library/Random Library/Coordinate Library/Sample Library/FrameBuffer Library/Array  Library/Exception)
foreach ext (hpp cpp)

svn cp ${rainbow_dir}/${class}.${ext} ${dst_dir}/${class}.${ext};

end
end

foreach class () #(Library/MDHash Library/PFT Library/Utility)
foreach ext (hpp cpp)

svn cp ${pdm_dir}/${class}.${ext} ${dst_dir}/${class}.${ext};

end
end


foreach main () #(Main/Stub)
foreach ext (cpp vcproj)

svn cp ${pdm_dir}/${main}.${ext} ${dst_dir}/${main}.${ext};

end
end




