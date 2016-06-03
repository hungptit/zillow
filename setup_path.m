function setup_path()
    currentDir = pwd;
    matlabSqlite3 = fullfile(currentDir, '..', '..', 'sqlite3');
    addpath(currentDir);
    addpath(matlabSqlite3);
end
