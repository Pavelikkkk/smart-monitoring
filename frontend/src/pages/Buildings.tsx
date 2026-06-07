import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { getBuildings } from "../services/api";

export default function Buildings() {
  const [buildings, setBuildings] =
    useState<any[]>([]);

  useEffect(() => {
    getBuildings()
      .then(setBuildings)
      .catch(console.error);
  }, []);

  return (
    <>
      <h1 className="text-4xl font-bold mb-8">
        Buildings
      </h1>

      <div className="grid grid-cols-1 xl:grid-cols-2 gap-6">

        {buildings.map((building) => (

          <Link
            key={building.id}
            to={`/buildings/${building.id}`}
            className="
              block
              bg-slate-800
              border
              border-slate-700
              rounded-xl
              p-6
              hover:border-orange-200
              transition
            "
          >
            <h2 className="text-2xl font-bold text-orange-200 mb-2">
              {building.name}
            </h2>

            <p className="text-slate-400 mb-2">
              {building.address}
            </p>

            <p className="text-slate-300">
              {building.description}
            </p>
          </Link>

        ))}

      </div>
    </>
  );
}