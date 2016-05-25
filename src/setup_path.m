function setup_path()
    currentDir = pwd;
    matlabSqlite3 = fullfile(currentDir, '..', '..', 'matlab-sqlite');
    addpath(currentDir);
    addpath(matlabSqlite3);
end
