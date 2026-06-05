import './App.css'

function App() {
  return (
    <div className="min-h-screen bg-slate-900 text-white p-8">
      <h1 className="text-5xl font-bold mb-8">
        Dorm Energy Monitoring
      </h1>

      <div className="grid grid-cols-3 gap-4">
        <div className="bg-slate-800 rounded-xl p-6">
          <h2 className="text-slate-400">Rooms</h2>
          <p className="text-3xl font-bold">6</p>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <h2 className="text-slate-400">Alerts</h2>
          <p className="text-3xl font-bold">3</p>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <h2 className="text-slate-400">ML Alerts</h2>
          <p className="text-3xl font-bold">1</p>
        </div>
      </div>
    </div>
  )
}

export default App